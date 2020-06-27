import socket
import threading
import random
import os
import time
import cv2

from Constants import Constants
from RtpPacket import RtpPacket

class RTPServerManager:
    '''
    RTP用于管理单一客户端的服务器类，每个类对应一个数据连接+控制连接，也对应Server中一个线程和一个服务器
    '''
    def __init__(self, ControlSocket, ClientAddress):
        '''
        描述：初始化类，获取对应的控制连接和服务器信息，初始化控制连接和一些变量
        参数：控制连接，服务器地址
        返回：无
        '''
        #是否有效，在teardown中设置为0,0这个对象就没了
        self.Valid = True            

        #RTP传输状态
        self.RTPStatus = Constants.RTP_TRANSPORT_INIT

        #数据包和控制包的seq
        self.ControlSequence = 0
        self.DataSequence = 0

        #控制连接，数据连接，客户端IP，服务器和客户端的两个端口       
        self.ControlSocket = ControlSocket
        self.ClientIP = ClientAddress[0]
        self.ClientControlPort = ClientAddress[1]
        self.ServerControlPort = Constants.RTP_SERVER_CONTROL_PORT
        self.DataSocket = None
        self.ServerDataPort = Constants.UNDEFINED_NUMBER
        self.ClientDataPort = Constants.UNDEFINED_NUMBER

        #session
        self.Session = Constants.UNDEFINED_NUMBER

        #文件信息
        self.ServerDir = "ServerDir"
        self.CurrentFileName = ""
        self.BufferImageDir = "BufferImage"
        self.BufferImageBack = ".jpg"
        self.FrameRate = Constants.UNDEFINED_NUMBER
        self.TotalFrameNumber = Constants.UNDEFINED_NUMBER

        #GBN用
        self.WindowSize = Constants.GBN_WINDOW_SIZE
        self.TimeOutTime = Constants.TIMEOUT_TIME

        #开始传输的图片帧序号
        self.StartPlace = Constants.UNDEFINED_NUMBER

        #进入循环接受指令状态
        self.ReceiveRTSPCommand()
    
    def InitializeDataPort(self):
        '''
        描述：初始化数据连接
        参数：控制连接，服务器地址
        返回：无
        '''
        self.DataSocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.DataSocket.settimeout(Constants.TIMEOUT_TIME)
        while True:
            try:
                self.ServerDataPort = self.GenerateRandomPort()
                self.DataSocket.bind(("", self.ServerDataPort))
                break
            except:
                continue

    #RTSP请求的读取，解析，判断合法性，回复等函数
    def ReceiveRTSPCommand(self):
        '''
        描述：一直接收RTSP指令，当teardown时关闭rtsp请求，然后结束
        参数：无
        返回：无
        '''
        while True:
            if self.Valid == False:
                break
            TheCommand = {}
            RawCommand = self.ControlSocket.recv(Constants.CONTROL_SIZE)
            print(RawCommand.decode("utf-8"))
            print("-----------------------------")
            if RawCommand: 
                TheCommand = self.ParseRTSPCommand(RawCommand.decode("utf-8"))
                self.HandleRTSPCommand(TheCommand)
        try:
            self.ControlSocket.close()
        except:
            donothing = 1
        return

    def SendBackReply(self, TheReply):
        '''
        描述：发送RTSP回复
        参数：无
        返回：无
        '''
        self.ControlSocket.send(TheReply.encode())
        return

    def HandleRTSPCommand(self, TheCommand):
        '''
        描述：中央控制，分别处理RTSP指令
        参数：RTSP指令
        返回：无
        '''
        if "Type" not in TheCommand:
            return
        try:
            Success = True
            TheInfo = TheCommand["Info"]
            TheSequence = TheCommand["Sequence"]
            TheSession = ""
            TheFileName = ""
            TheMessage = ""
            TheReply = ""
            TheAdditionalInfo = ""

            #验证session
            if "Session" in TheCommand:
                TheSession = TheCommand["Session"]
                if self.Session == Constants.UNDEFINED_NUMBER:
                    self.Session = TheSession
                if TheSession != self.Session:
                    Success = False
                    TheMessage = "Invalid Session!"
            else:
                Success = False
                TheMessage = "Invalid Session!"   

            #验证seq
            if TheSequence == self.ControlSequence + 1:
                self.ControlSequence = TheSequence
            else:
                Success = False
                TheMessage = "Invalid Sequence!"
            
            if "FileName" in TheCommand:
                TheFileName = TheCommand["FileName"]
            if Success:
                if TheCommand["Type"] == "SETUP":
                    Success, TheMessage = self.HandleSetup(self.GetRealFileName(TheFileName), TheCommand["Port"])
                elif TheCommand["Type"] == "PLAY":
                    Success, TheMessage = self.HandlePlay()
                elif TheCommand["Type"] == "PAUSE":
                    Success, TheMessage = self.HandlePause()
                elif TheCommand["Type"] == "RESUME":
                    Success, TheMessage = self.HandleResume()
                elif TheCommand["Type"] == "TEARDOWN":
                    Success, TheMessage = self.HandleTearDown()
                elif TheCommand["Type"] == "GET_PARAMETER":
                    Success, TheMessage, TheAdditionalInfo = self.HandleGetParameter()
                elif TheCommand["Type"] == "SET_START_PLACE":
                    Success, TheMessage = self.HandleSetStartPlace(TheCommand["StartPlace"])

            TheReply = self.GenerateRTSPReply(TheInfo, Success, TheMessage, TheSequence, TheSession, TheAdditionalInfo)
            print(TheReply)
            print("-----------------------------")
            self.SendBackReply(TheReply)
            return
        except:
            return
            
    def ParseRTSPCommand(self, RawCommand):
        '''
        描述：解析RTSP指令
        参数：指令字符串
        返回：RTSP指令，非法就是空字典
        '''
        try:
            LineList = RawCommand.split('\n')
            TheCommand = {}
            for i in range(len(LineList)):
                item = LineList[i]
                ItemList = item.split()
                if i == 0:
                    TheCommand["Type"] = ItemList[0]
                    TheCommand["Info"] = ItemList[2]
                    if TheCommand["Type"] in ["SETUP", "PLAY", "PAUSE", "RESUME", "TEARDOWN", "GET_PARAMETER", "SET_START_PLACE"]:
                        TheCommand["FileName"] = ItemList[1]
                elif i == 1:
                    TheCommand["Sequence"] = int(ItemList[1])
                elif i == 2:
                    if TheCommand["Type"] in ["SETUP", "PLAY", "PAUSE", "RESUME", "TEARDOWN", "GET_PARAMETER", "SET_START_PLACE"]:
                        TheCommand["Session"] = int(ItemList[1])
                elif i == 3:
                    if TheCommand["Type"] in ["SETUP"]:
                        TheCommand["Port"] = int(ItemList[3])
                    elif TheCommand["Type"] in ["SET_START_PLACE"]:
                        TheCommand["StartPlace"] = int(ItemList[1])
        except:
            TheCommand = {}
        return TheCommand
    
    def GenerateRTSPReply(self, TheInfo, WhetherSuccess, TheMessage, TheSequence, TheSession, TheAdditionalInfo):
        '''
        描述：生成RTSP回复
        参数：返回的RTSP类型，是否成功，顺序码，session
        返回：RTSP Reply字符串
        '''
        TheReply = ""
        TheStatusCode = 0
        if WhetherSuccess == True:
            TheStatusCode = Constants.STATUS_CODE_SUCCESS
        else:
            TheStatusCode = Constants.STATUS_CODE_FAIL
        TheReply = TheInfo + " " + str(TheStatusCode) + " " + TheMessage + "\n"\
        + "Seq:" + " " + str(TheSequence) + "\n"\
        + "Session:" + " " + str(TheSession) + TheAdditionalInfo
        return TheReply


    #处理RTSP请求的函数
    def HandleSetup(self, TheFileName, TheDataPort):
        '''
        描述：处理setup请求，也就是建立连接的第一步，这里要获取待播放的文件名和rtp连接端口等
        参数：文件名，数据连接端口
        返回：第一个参数T/F代表是否成功，第二个参数代表消息
        '''
        self.ClientDataPort = TheDataPort
        self.CurrentFileName = TheFileName
        self.RTPStatus = Constants.RTP_TRANSPORT_READY
        return True, "OK"

    def HandlePlay(self):
        '''
        描述：处理play请求，也就是开始播放，这里要新建一个线程，新建rtp连接并且发送数据
        参数：文件名
        返回：第一个参数T/F代表是否成功，第二个参数代表消息
        '''
        if self.RTPStatus == Constants.RTP_TRANSPORT_READY:
            self.RTPStatus = Constants.RTP_TRANSPORT_PLAYING
            NewThread = threading.Thread(target = self.RTPSend, args = ())  #t为新创建的线程
            NewThread.start()
            return True, "OK"
        else:
            if self.RTPStatus == Constants.RTP_TRANSPORT_INIT:
                return False, Constants.RTP_ERROR_INIT
            else:
                return False, Constants.RTP_ERROR_PLAYING

    def HandlePause(self):
        '''
        描述：处理pause请求，也就是暂停播放
        参数：文件名
        返回：第一个参数T/F代表是否成功，第二个参数代表消息
        '''
        if self.RTPStatus == Constants.RTP_TRANSPORT_PLAYING:
            self.RTPStatus = Constants.RTP_TRANSPORT_READY
            return True, "OK"
        else:
            if self.RTPStatus == Constants.RTP_TRANSPORT_INIT:
                return False, Constants.RTP_ERROR_INIT
            else:
                return False, Constants.RTP_ERROR_READY
        return False, "Not Implemented"        
    
    def HandleResume(self):
        '''
        描述：处理resume请求，也就是继续播放
        参数：文件名
        返回：第一个参数T/F代表是否成功，第二个参数代表消息
        '''
        if self.RTPStatus == Constants.RTP_TRANSPORT_READY:
            self.RTPStatus = Constants.RTP_TRANSPORT_PLAYING
            return True, "OK"
        else:
            if self.RTPStatus == Constants.RTP_TRANSPORT_INIT:
                return False, Constants.RTP_ERROR_INIT
            else:
                return False, Constants.RTP_ERROR_PLAYING

    def HandleTearDown(self):
        '''
        描述：处理teardown请求，也就是关闭这个数据和控制连接
        参数：无
        返回：第一个参数T/F代表是否成功，第二个参数代表消息
        '''
        if self.DataSocket != None:
            try:
                self.DataSocket.shutdown(SHUT_RDWR)
                self.DataSocket.close()
            except:
                donothing = 1
        self.Valid = False
        return True, "OK"

    def HandleGetParameter(self):
        '''
        描述：处理get parameter请求，也就是获取视频基本信息
        参数：无
        返回：第一个参数T/F代表是否成功，第二个参数代表消息, 第三个参数代表额外的行---返回的视频信息
        '''
        TheFrameNumber, TheFrameRate, TheFrameWidth, TheFrameHeight = self.GetVideoInfo()
        TheAdditionalInfo = "\n" + "FrameNumber: " + str(TheFrameNumber) + " FrameRate: " + str(TheFrameRate)\
        + " FrameWidth: " + str(TheFrameWidth) + " FrameHeight: " + str(TheFrameHeight)
        return True, "OK", TheAdditionalInfo

    def HandleSetStartPlace(self, TheStartPlace):
        '''
        描述：处理setstartplace请求，也就是设置初始传输位置
        参数：无
        返回：第一个参数T/F代表是否成功，第二个参数代表消息
        '''
        if self.RTPStatus == Constants.RTP_TRANSPORT_READY:
            if TheStartPlace < 0:
                TheStartPlace = 0
            if TheStartPlace >= self.TotalFrameNumber:
                TheStartPlace = self.TotalFrameNumber - 1
            self.StartPlace = TheStartPlace
            return True, "OK"
        else:
            if self.RTPStatus == Constants.RTP_TRANSPORT_INIT:
                return False, Constants.RTP_ERROR_INIT
            else:
                return False, Constants.RTP_ERROR_PLAYING


    #RTP发送数据的函数
    def RTPSend(self):
        '''
        描述：多线程发送文件
        参数：无
        返回：无
        '''
        #初始化网络，待传输视频，文件夹等
        self.InitializeDataPort()
        self.PrepareBufferPlace()
        print("A new Process of Client (", self.ClientIP,",",self.ClientControlPort,") has opened for RTP")
        print("The new Data Port is ", self.ClientDataPort)
        print("The Server Data Port is ", self.ServerDataPort)
        Success = True
        TheVideo = cv2.VideoCapture(self.CurrentFileName)
        if TheVideo.isOpened():
            WhetherRemain, TheFrame = TheVideo.read()
        else:
            WhetherRemain = False

        for i in range(self.StartPlace):
            WhetherRemain, TheFrame = TheVideo.read()

        #状态判断等
        if Success == True and WhetherRemain == True:
            while WhetherRemain == True:
                if self.Valid != True:
                        break
                if self.RTPStatus == Constants.RTP_TRANSPORT_PLAYING:
                    #读取一帧视频,并且GBN发送
                    WhetherRemain = self.CreateBufferImage(TheVideo)
                    if WhetherRemain == False:
                        break
                    if self.SendOnePictureGBN() == False:
                        break     

        TheVideo.release()
        print("The RTP Data Process of Client (", self.ClientIP,",",self.ClientControlPort,") has closed")
        return        

    def SendRTPPacket(self, TheData, WhetherFirst, TheDataSequence):
        '''
        描述：将一个数据打包成RTP包，然后发送出去
        参数：数据，是否第一个，seq
        返回：无
        '''
        ThePacket = RtpPacket()
        if WhetherFirst:
            TheMarker = 0
        else:
            TheMarker = 1
        ThePacket.encode(Constants.RTP_CURRENT_VERSION, Constants.RTP_PADDLING_FALSE, Constants.RTP_EXTENSION_FALSE, \
        Constants.RTP_CC, TheDataSequence, TheMarker, Constants.RTP_TYPE_JPEG, Constants.RTP_SSRC ,\
        TheData)
        TheSendData = ThePacket.getPacket()
        try:
            self.DataSocket.sendto(TheSendData,(self.ClientIP, self.ClientDataPort))
        except:
            donothing = True
        return


    #用于视频文件和视频信息读取的函数
    def CreateBufferImage(self, TheVideo):
        '''
        描述：读取视频一帧，生成缓存文件
        参数：视频
        返回：是否remain，是true否false
        '''
        TheBufferImageName = self.GetBufferImageName()
        try:
            os.remove(TheBufferImageName)
        except:
            donothing = True
        WhetherRemain, TheFrame = TheVideo.read()
        cv2.imwrite(TheBufferImageName, TheFrame)
        #cv2.waitKey(1)
        return WhetherRemain

    def GetVideoInfo(self):
        '''
        描述：获取视频的帧率,总帧数,宽度，高度
        参数：无
        返回：帧数，帧率，宽度，高度
        '''
        TheVideo = cv2.VideoCapture(self.CurrentFileName)
        TheFrameRate = round(TheVideo.get(cv2.CAP_PROP_FPS))
        TheFrameNumber = round(TheVideo.get(cv2.CAP_PROP_FRAME_COUNT))
        TheFrameWidth = round(TheVideo.get(cv2.CAP_PROP_FRAME_WIDTH))
        TheFrameHeight = round(TheVideo.get(cv2.CAP_PROP_FRAME_HEIGHT))
        self.TotalFrameNumber = TheFrameNumber
        self.FrameRate = TheFrameRate
        return TheFrameNumber, TheFrameRate, TheFrameWidth, TheFrameHeight


    #关于GBN发送的函数
    def SendOnePictureGBN(self):
        '''
        描述：GBN发送一张图
        参数：无
        返回：成功true失败false
        '''
        Success = True
        TheDataList = []
        Success, TheDataList = self.PartitionOnePicture()
        if Success == False:
            return False
        TotalNumber = len(TheDataList)
        CurrentPlace = 0
        #print(CurrentPlace, TotalNumber)
        #循环发送窗口
        while CurrentPlace < TotalNumber:
            i = 0
            if self.Valid == False:
                return False
            #一个发送窗口
            while CurrentPlace + i < TotalNumber and i < self.WindowSize:
                TheSequenceNum = self.DataSequence + 1 + CurrentPlace + i
                if CurrentPlace + i == 0:
                    WhetherFirst = True
                else:
                    WhetherFirst = False
                #print("Sending ", TheSequenceNum)
                self.SendRTPPacket(TheDataList[CurrentPlace + i], WhetherFirst, TheSequenceNum)
                i = i + 1
            TheMaxACK = self.ReceiveACK(self.DataSequence + CurrentPlace)
            #print(TheMaxACK)
            if TheMaxACK - self.DataSequence > CurrentPlace:
                CurrentPlace = TheMaxACK - self.DataSequence
        self.DataSequence = self.DataSequence + TotalNumber
        return True

    def ReceiveACK(self, PreviousACKNumber):
        '''
        描述：接收GBN发送的ACK
        参数：之前ACK过的最大数
        返回：这次ACK的最大数
        '''
        MaxACKNumber = PreviousACKNumber
        self.DataSocket.settimeout(self.TimeOutTime)
        for i in range(self.WindowSize):
            try:
                Data = self.DataSocket.recv(Constants.CONTROL_SIZE).decode()
                #print(Data)
                TheACKNumber = int(Data[4:])
                if TheACKNumber > MaxACKNumber:
                    MaxACKNumber = TheACKNumber
            except:
                donothing = True
        #print(MaxACKNumber)
        return MaxACKNumber

    def PartitionOnePicture(self):
        '''
        描述：把视频一帧拆分为若干数据包
        参数：无
        返回：是否成功，数据包数组
        '''
        Success = True   
        TheBufferImageName = self.GetBufferImageName()
        try:
            File = open(TheBufferImageName, 'rb')
        except:
            Success = False
            return False
        DataList = []
        while True:
            #循环读取一帧并且发送
            TheData = File.read(Constants.DATA_PACKET_SIZE - Constants.DATA_HEADER_SIZE)
            if len(TheData) == 0:
                break
            else:
                DataList.append(TheData)
        File.close()
        return Success, DataList

    #通用函数，用于生成端口，session，文件名处理等
    def GenerateSession(self):
        '''
        描述：生成随机的session
        参数：无
        返回：一个随机数session
        '''
        TheSession = random.randint(0, Constants.MAX_NUMBER)
        return TheSession

    def GenerateRandomPort(self):
        '''
        描述：生成随机的服务器数据端口
        参数：无
        返回：一个随机数port
        '''
        ThePort = random.randint(10001, 65535)
        return ThePort

    def GetRealFileName(self, TheFileName):
        '''
        描述：获取服务器的实际文件名
        参数：请求文件名
        返回：实际文件名
        '''
        RealFileName = self.ServerDir + '/' + TheFileName
        return RealFileName

    def PrepareBufferPlace(self):
        '''
        描述：生成缓存文件夹
        参数：无
        返回：无
        '''
        if not os.path.exists(self.BufferImageDir):
            os.mkdir(self.BufferImageDir)

    def GetBufferImageName(self):
        '''
        描述：生成缓存文件名
        参数：无
        返回：无
        '''
        return self.BufferImageDir + '/' + str(self.Session) + self.BufferImageBack

