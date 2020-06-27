import socket
import threading
import random
import os
import time
from Constants import Constants
from RtpPacket import RtpPacket

class ServerManager:
    '''
    用于管理单一客户端的服务器类，每个类对应一个数据连接+控制连接，也对应Server中一个线程和一个服务器
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
        self.ServerControlPort = Constants.SERVER_CONTROL_PORT
        self.DataSocket = None
        self.ServerDataPort = Constants.UNDEFINED_NUMBER
        self.ClientDataPort = Constants.UNDEFINED_NUMBER

        #session
        self.Session = Constants.UNDEFINED_NUMBER

        #当前要打开的文件
        self.CurrentFileName = ""

        #进入循环接受指令状态
        self.ReceiveRTSPCommand()
    
    def InitializeDataPort(self):
        '''
        描述：初始化数据连接
        参数：控制连接，服务器地址
        返回：无
        '''
        self.DataSocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.DataSocket.settimeout(0.5)
        while True:
            try:
                self.ServerDataPort = self.GenerateRandomPort()
                self.DataSocket.bind(("", self.ServerDataPort))
                break
            except:
                continue

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

            #验证session
            if TheCommand["Type"] != "SETUP":
                if "Session" in TheCommand:
                    TheSession = TheCommand["Session"]
                    if TheSession != self.Session:
                        Success = False
                        TheMessage = "Invalid Session!"
                else:
                    Success = False
                    TheMessage = "Invalid Session!"   
            else:
                TheSession = self.GenerateSession()
                self.Session = TheSession

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
                    Success, TheMessage = self.HandleSetup(TheFileName, TheCommand["Port"])
                elif TheCommand["Type"] == "PLAY":
                    Success, TheMessage = self.HandlePlay()
                elif TheCommand["Type"] == "PAUSE":
                    Success, TheMessage = self.HandlePause()
                elif TheCommand["Type"] == "TEARDOWN":
                    Success, TheMessage = self.HandleTearDown()
            TheReply = self.GenerateRTSPReply(TheInfo, Success, TheMessage, TheSequence, TheSession)
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
                    if TheCommand["Type"] in ["SETUP", "PLAY", "PAUSE", "TEARDOWN"]:
                        TheCommand["FileName"] = ItemList[1]
                elif i == 1:
                    TheCommand["Sequence"] = int(ItemList[1])
                elif i == 2:
                    if TheCommand["Type"] in ["SETUP"]:
                        TheCommand["Port"] = int(ItemList[3])
                    elif TheCommand["Type"] in ["PLAY", "PAUSE", "TEARDOWN"]:
                        TheCommand["Session"] = int(ItemList[1])
        except:
            TheCommand = {}
        return TheCommand
    
    def GenerateRTSPReply(self, TheInfo, WhetherSuccess, TheMessage, TheSequence, TheSession):
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
        + "Session:" + " " + str(TheSession)
        return TheReply

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
        #if self.RTPStatus == Constants.RTP_TRANSPORT_PLAYING:
        #    self.RTPStatus = Constants.RTP_TRANSPORT_READY
        #    return True, "OK"
        #else:
        #    if self.RTPStatus == Constants.RTP_TRANSPORT_INIT:
        #        return False, Constants.RTP_ERROR_INIT
        #    else:
        #        return False, Constants.RTP_ERROR_READY
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

    def RTPSend(self):
        '''
        描述：多线程发送文件
        参数：无
        返回：无
        '''
        self.InitializeDataPort()
        print("A new Process of Client (", self.ClientIP,",",self.ClientControlPort,") has opened for RTP")
        print("The new Data Port is ", self.ClientDataPort)

        Success = True
        
        #print(Success)

        if Success == True:
            for i in range(Constants.FILE_NUMBER):
                print("It is sending the " + str(i) + " th picture.")
                WhetherFirst = True
                if self.Valid != True:
                    break
                if self.RTPStatus == Constants.RTP_TRANSPORT_PLAYING:
                    TheEntireFileName = self.CurrentFileName + str(i) + '.jpg'
                    try:
                        File = open(TheEntireFileName, 'rb')
                    except:
                        Success = False
                        break
                    while True:
                        TheData = File.read(Constants.DATA_PACKET_SIZE - Constants.DATA_HEADER_SIZE)
                        time.sleep(0.1)

                        if len(TheData) == 0:
                            break
                        self.SendRTPPacket(TheData, WhetherFirst)
                        WhetherFirst = False
                    File.close()
        print("The RTP Data Process of Client (", self.ClientIP,",",self.ClientControlPort,") has closed")
        return        

    def SendRTPPacket(self, TheData, WhetherFirst):
        '''
        描述：将一个数据打包成RTP包，然后发送出去
        参数：无
        返回：无
        '''
        ThePacket = RtpPacket()
        self.DataSequence = self.DataSequence + 1
        if WhetherFirst:
            TheMarker = 0
        else:
            TheMarker = 1
        ThePacket.encode(Constants.RTP_CURRENT_VERSION, Constants.RTP_PADDLING_FALSE, Constants.RTP_EXTENSION_FALSE, \
        Constants.RTP_CC, self.DataSequence, TheMarker, Constants.RTP_TYPE_JPEG, Constants.RTP_SSRC ,\
        TheData)
        TheSendData = ThePacket.getPacket()
        try:
            self.DataSocket.sendto(TheSendData,(self.ClientIP, self.ClientDataPort))
        except:
            donothing = True
        return