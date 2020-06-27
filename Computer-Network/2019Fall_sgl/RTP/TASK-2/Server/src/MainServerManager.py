import socket
import threading
import random
import os
import time
import cv2

from Constants import Constants
from RtpPacket import RtpPacket

class MainServerManager:
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

        #seq
        self.ControlSequence = 0

        #控制连接，数据连接，客户端IP，服务器和客户端的两个端口      
        self.ControlSocket = ControlSocket
        self.ClientIP = ClientAddress[0]
        self.ClientControlPort = ClientAddress[1]
        self.ServerControlPort = Constants.MAIN_SERVER_CONTROL_PORT
        self.DataSocket = None
        self.ServerDataPort = Constants.UNDEFINED_NUMBER
        self.ClientDataPort = Constants.UNDEFINED_NUMBER

        #session
        self.Session = Constants.UNDEFINED_NUMBER

        #文件信息
        self.ServerDir = "ServerDir"
        self.CurrentFileName = ""
        self.PictureBack = ".jpg"
        self.SubtitleBack = ".srt"

        #进入循环接受指令状态
        self.ReceiveControlCommand()

    #控制连接相关函数
    def ReceiveControlCommand(self):
        '''
        描述：一直接收控制指令，当teardown时关闭控制连接，然后结束
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
                TheCommand = self.ParseControlCommand(RawCommand.decode("utf-8"))
                self.HandleControlCommand(TheCommand)
        try:
            self.ControlSocket.close()
        except:
            donothing = 1
        return

    def SendBackReply(self, TheReply):
        '''
        描述：发送控制回复
        参数：无
        返回：无
        '''
        self.ControlSocket.send(TheReply.encode())
        return

    def HandleControlCommand(self, TheCommand):
        '''
        描述：中央控制，分别处理控制指令
        参数：控制指令
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
            if TheCommand["Type"] != "SETUP":
                if "Session" in TheCommand:
                    TheSession = TheCommand["Session"]
                    if TheSession != self.Session:
                        Success = False
                        TheMessage = "Invalid Session!"
            else:
                self.Session = self.GenerateSession() 
                TheSession = self.Session 

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
                    Success, TheMessage = self.HandleSetup(TheCommand["Port"])
                elif TheCommand["Type"] == "LIST":
                    Success, TheMessage, TheAdditionalInfo = self.HandleList()
                elif TheCommand["Type"] == "PORT":
                    Success, TheMessage = self.HandlePort(TheFileName)
                elif TheCommand["Type"] == "RETR":
                    Success, TheMessage = self.HandleDownload(TheFileName)
                elif TheCommand["Type"] == "TEARDOWN":
                    Success, TheMessage = self.HandleTearDown()

            TheReply = self.GenerateControlReply(TheInfo, Success, TheMessage, TheSequence, TheSession, TheAdditionalInfo)
            print(TheReply)
            print("-----------------------------")
            self.SendBackReply(TheReply)
            return
        except:
            return
            
    def ParseControlCommand(self, RawCommand):
        '''
        描述：解析控制指令
        参数：指令字符串
        返回：控制指令，非法就是空字典
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
                    if TheCommand["Type"] in ["SETUP", "LIST", "RETR", "TEARDOWN", "PORT"]:
                        TheCommand["FileName"] = ItemList[1]
                elif i == 1:
                    TheCommand["Sequence"] = int(ItemList[1])
                elif i == 2:
                    if TheCommand["Type"] in ["SETUP"]:
                        TheCommand["Port"] = int(ItemList[3])
                    elif TheCommand["Type"] in ["LIST", "RETR", "TEARDOWN", "PORT"]:
                        TheCommand["Session"] = int(ItemList[1])
        except:
            TheCommand = {}
        return TheCommand
    
    def GenerateControlReply(self, TheInfo, WhetherSuccess, TheMessage, TheSequence, TheSession, TheAdditionalInfo):
        '''
        描述：生成控制回复
        参数：返回的控制类型，是否成功，顺序码，session
        返回：Reply字符串
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

    #全局函数，如获取session，端口，文件名等
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

    def GetCoverImageName(self, TheFileName):
        '''
        描述：获取服务器的实际封面图片文件名
        参数：请求文件名
        返回：实际封面文件名
        '''
        CoverFileName = self.ServerDir + '/' + TheFileName[:-4] + self.PictureBack
        return CoverFileName

    def GetSubtitleName(self, TheFileName):
        '''
        描述：获取服务器的实际字幕文件名
        参数：请求文件名
        返回：实际字幕文件名
        '''
        SubtitleName = self.ServerDir + '/' + TheFileName[:-4] + self.SubtitleBack
        return SubtitleName

    #处理请求的相关函数
    def HandleSetup(self, TheDataPort):
        '''
        描述：处理setup请求，也就是建立连接的第一步，这里要获取rtp连接端口等
        参数：数据连接端口
        返回：第一个参数T/F代表是否成功，第二个参数代表消息
        '''
        self.ClientDataPort = TheDataPort
        return True, "OK"

    def HandleList(self):
        '''
        描述：处理list请求，也就是发送全部视频文件信息和封面，字幕
        参数：无
        返回：第一个参数T/F代表是否成功，第二个参数代表消息，第三个参数代表视频文件列表，每三个元素代表名称，帧数，帧率
        '''
        TheFileList = "\n"
        for root, dirs, files in os.walk(self.ServerDir):
            for file in files:
                if os.path.splitext(file)[1] in ['.mp4','.flv','.avi']:
                    TheFileName = str(file)
                    #print(TheFileName)
                    if self.CreateCoverImage(TheFileName) == True:
                        TheFrameNumber, TheFrameRate = self.GetVideoInfo(TheFileName)
                        TheFileList = TheFileList + TheFileName + ' ' + str(TheFrameNumber) + ' ' + str(TheFrameRate) + ' '
        return True, "OK", TheFileList

    def HandlePort(self, TheFileName):
        '''
        描述：处理下载前确认请求，也就是确认是否有文件
        参数：无
        返回：第一个参数T/F代表是否成功，第二个参数代表消息
        '''
        TheRealFileName = self.GetRealFileName(TheFileName)
        #print(TheRealFileName)
        try:
            File = open(TheRealFileName, 'rb')
            File.close()
            return True, "OK"
        except:
            return False, "File not found!"

    def HandleDownload(self, TheFileName):
        '''
        描述：处理下载请求，也就是发送封面/字幕
        参数：无
        返回：第一个参数T/F代表是否成功
        '''
        TheRealFileName = self.GetRealFileName(TheFileName)
        self.CurrentFileName = TheRealFileName
        threading.Thread(target = self.SendData).start()
        return True, "OK"

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

    #用于视频文件和视频信息读取的函数
    def CreateCoverImage(self, TheFileName):
        '''
        描述：读取视频第一帧，生成封面图片
        参数：请求视频文件名
        返回：是否remain，是true否false
        '''
        TheVideo = cv2.VideoCapture(self.GetRealFileName(TheFileName))
        if TheVideo.isOpened():
            WhetherRemain, TheFrame = TheVideo.read()
        else:
            WhetherRemain = False
        TheCoverImageName = self.GetCoverImageName(TheFileName)
        if os.path.exists(TheCoverImageName) != True:
            TheFrame = cv2.resize(TheFrame, (150, 150))
            cv2.imwrite(TheCoverImageName, TheFrame)
        TheVideo.release()
        #cv2.waitKey(1)
        return WhetherRemain

    def GetVideoInfo(self, TheFileName):
        '''
        描述：获取视频的帧率,总帧数
        参数：请求视频文件名
        返回：帧数，帧率
        '''
        TheVideo = cv2.VideoCapture(self.GetRealFileName(TheFileName))
        TheFrameRate = round(TheVideo.get(cv2.CAP_PROP_FPS))
        TheFrameNumber = round(TheVideo.get(cv2.CAP_PROP_FRAME_COUNT))
        return TheFrameNumber, TheFrameRate

    #数据连接函数
    def OpenDataPort(self):
        '''
        描述：开启数据连接
        参数：请求视频文件名
        返回：帧数，帧率
        '''
        self.DataSocket = socket.socket()
        self.DataSocket.connect((self.ClientIP, self.ClientDataPort))

    def CloseDataPort(self):
        '''
        描述：开启控制连接
        参数：请求视频文件名
        返回：帧数，帧率
        '''
        try:
            self.DataSocket.shutdown(SHUT_RDWR)
            self.DataSocket.close()
        except:
            donothing = True

    def SendData(self):
        '''
        描述：控制连接发送数据
        参数：请求视频文件名
        返回：帧数，帧率
        '''
        print("Opening data port to send to ", self.ClientIP, self.ClientDataPort)
        self.OpenDataPort()
        File = open(self.CurrentFileName, 'rb')
        Data = File.read(Constants.DATA_PACKET_SIZE)
        a = self.DataSocket.send(Data)             
        File.close()
        print("Closing data port to send to ", self.ClientIP, self.ClientDataPort)