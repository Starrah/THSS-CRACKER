from tkinter import *
import socket
import sys
import os
import random
from math import *
import time
import threading
import re
from PyQt5.QtWidgets import QApplication, QMainWindow, QInputDialog, QFileDialog, QPushButton, \
QLineEdit, QTableWidgetItem, QMessageBox, QProgressBar, QMenu, QAbstractItemView, QListView, QListWidget, QListWidgetItem
from PyQt5.QtCore import QObject, Qt, QThread, pyqtSignal, QSize
from PyQt5.uic import loadUi
from PyQt5.QtGui import QIcon

from Constants import Constants
from PlayClient import PlayClient


class MainClient(QObject):
    '''
    描述：pyqt主客户端, 类似FTP客户端
    '''
    InitializeFinished = pyqtSignal()


    def __init__(self):
        '''
        描述：构造函数
        参数：无
        '''
        super(QObject,self).__init__()

        #ip，端口等
        self.IP = ""
        self.ClientControlPort = Constants.UNDEFINED_NUMBER
        self.ClientDataPort = self.GenerateRandomPort()
        self.ServerIP = Constants.SERVER_ADDR
        self.ServerControlPort = Constants.MAIN_SERVER_CONTROL_PORT

        #连接
        self.ControlSocket = None
        self.ListenSocket = None
        self.DataSocket = None

        #文件
        self.FileName = "kebab.jpg"
        self.SaveDir = "Info"
        self.PictureBack = ".jpg"
        self.SubtitleBack = ".srt"

        #文件列表
        self.PlayList = []
        self.DownloadList = []
        self.DownloadPlace = 0
        #元素：文件名，时长，已经播放时长，帧数，已经播放帧数，是否有字幕

        #状态
        self.WhetherPlaying = False
        self.Valid = True
        self.WhetherSendingFinished = False
        self.WhetherSendingSuccess = True
        self.RequestSent = ""


        #session，seq等
        self.ControlSequence = 0
        self.Session = Constants.UNDEFINED_NUMBER

        #初始化ui
        self.LoginWindow = loadUi("LoginWindow.ui")
        self.MainWindow = loadUi("mainwindow.ui")
        self.LoginWindow.show()
        self.ConnectSignalAndSlot()
    

	#网络连接相关操作，包括数据端口连接服务器，控制端口开启等
    def Initialize(self):
        '''
        描述：初始化函数，负责在登录窗口连接服务器，下载一系列东西，最后进入主界面
        参数：无
        返回：无
        '''	
        self.ServerIP = self.LoginWindow.IPText.toPlainText() 
        self.ConnectToServer()
        self.OpenDataPort()
        self.SetupLink()    

    def ConnectToServer(self):
        '''
        描述：控制连接服务器
        参数：无
        返回：无
        '''	
        self.ControlSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.ControlSocket.settimeout(2)
        try:
            self.ControlSocket.connect((self.ServerIP, self.ServerControlPort))
        except:
            QMessageBox.warning(self.MainWindow,'Connection Fail',"Unable to connect to the Server",QMessageBox.Yes)
            self.LoginWindow.close()
            self.MainWindow.close()	


    def OpenDataPort(self):
        '''
        描述：开启数据端口，接收服务器数据
        参数：无
        返回：无
        '''	
        self.ListenSocket = socket.socket()
        try:
            self.ListenSocket.bind(("", self.ClientDataPort))
            self.ListenSocket.listen(5)
        except:
            QMessageBox.warning(self.MainWindow,'Connection Fail',"Unable to bind the data port",QMessageBox.Yes)

    def CloseDataPort(self):
        '''
        描述：关闭数据端口
        参数：无
        返回：无
        '''	
        try:
            self.DataSocket.shutdown(socket.SHUT_RDWR)
            self.DataSocket.close()
        except:
            donothing = True

	#绑定事件处理相关操作，包括建立连接，获取文件列表，建立数据连接，下载文件，关闭客户端
    def SetupLink(self):
        '''
        描述：Setup操作
        参数：无
        返回：无
        '''		
        self.SendControlRequest("SETUP")
	
    def TearDownLink(self):
        '''
        描述：Teardown操作，关闭连接
        参数：无
        返回：无
        '''	
        self.SendControlRequest("TEARDOWN")		

         
    def GetPlayList(self):
        '''
        描述：获取视频文件列表
        参数：无
        返回：无
        '''	
        self.SendControlRequest("LIST")

    def BeforeDownload(self):
        '''
        描述：下载一个文件前做好准备
        参数：无
        返回：无
        '''	
        self.WhetherSendingFinished = False
        self.SendControlRequest("PORT")

    def DownloadFile(self):
        '''
        描述：下载一个文件
        参数：无
        返回：无
        '''	
        self.SendControlRequest("RETR")


	#控制连接相关函数，包括发送请求，处理收到的回复，处理请求等
    def SendControlRequest(self, TheRequestType):
        '''
        描述：向服务器发送控制请求
        参数：请求类型
        返回：无
        '''	
        if TheRequestType == "SETUP":
            self.ControlSequence += 1
            threading.Thread(target = self.ReceiveControlReply).start()
            TheRequest = 'SETUP ' + self.FileName + ' RTSP/1.0\n' \
            + 'CSeq: ' + str(self.ControlSequence) + \
            '\nTransport: TCP; client_port= ' + str(self.ClientDataPort)
            self.RequestSent = "SETUP"
		
        elif TheRequestType == "LIST":
            self.ControlSequence += 1
            TheRequest = 'LIST ' + self.FileName + ' RTSP/1.0\n' \
            + 'CSeq: ' + str(self.ControlSequence) \
            + '\nSession: ' + str(self.Session)
            self.RequestSent = "LIST"

        elif TheRequestType == "PORT":
            self.ControlSequence += 1
            TheRequest = 'PORT ' + self.FileName + ' RTSP/1.0\n' \
            + 'CSeq: ' + str(self.ControlSequence) \
            + '\nSession: ' + str(self.Session)
            self.RequestSent = "PORT"

        elif TheRequestType == "RETR":
            self.ControlSequence += 1
            TheRequest = 'RETR ' + self.FileName + ' RTSP/1.0\n' \
            + 'CSeq: ' + str(self.ControlSequence) \
            + '\nSession: ' + str(self.Session)
            self.RequestSent = "RETR"
		
        elif TheRequestType == "TEARDOWN":
            self.ControlSequence += 1
            TheRequest = 'TEARDOWN ' + self.FileName + ' RTSP/1.0\n' \
            + 'CSeq: ' + str(self.ControlSequence) \
            + '\nSession: ' + str(self.Session)
            self.RequestSent = "TEARDOWN"
        else:
            return
        self.ControlSocket.send(TheRequest.encode())		
        print(TheRequest)
        print("-----------------------------")

	
    def ReceiveControlReply(self):
        '''
        描述：接收服务器的控制连接回复
        参数：无
        返回：无
        '''	
        while True:
            TheReply = self.ControlSocket.recv(Constants.CONTROL_SIZE)
            print(TheReply.decode("utf-8"))
            print("-----------------------------")
            if TheReply: 
                self.HandleControlReply(TheReply.decode("utf-8"))

            # Close the RTSP socket upon requesting Teardown
            if self.RequestSent == "TEARDOWN":
                try:
                    self.ControlSocket.shutdown(socket.SHUT_RDWR)
                    self.ControlSocket.close()
                except:
                    donothing = True
                break
	
    def HandleControlReply(self, TheReply):
        '''
        描述：处理服务器的控制连接回复
        参数：回复内容
        返回：无
        '''	
        Lines = str(TheReply).split('\n')
        TheSequenceNum = int(Lines[1].split()[1])
		
        # Process only if the server reply's sequence number is the same as the request's
        if TheSequenceNum == self.ControlSequence:
            TheSession = int(Lines[2].split()[1])
            # New RTSP session ID
            if self.Session == Constants.UNDEFINED_NUMBER:
                self.Session = TheSession
			
            # Process only if the session ID is the same
            if self.Session == TheSession:
                if int(Lines[0].split()[1]) == Constants.STATUS_CODE_SUCCESS: 
                    if self.RequestSent == "SETUP":
                        self.GetPlayList()
                        self.InitDir()
                    elif self.RequestSent == "LIST":
                        self.SetPlayList(str(TheReply))
                        self.GetAllFiles()
                    elif self.RequestSent == "PORT":
                        self.DownloadFile()
                        threading.Thread(target = self.DataLinkReceive).start()
                    elif self.RequestSent == "TEARDOWN":
                        self.Valid = False
                        self.InitializeFinished.emit()
                else:
                    if self.RequestSent == "PORT":
                        self.WhetherSendingFinished = True
                        self.WhetherSendingSuccess = False
                        if self.DownloadPlace % 2 == 0:
                            self.PlayList[round(self.DownloadPlace / 2)]["WhetherHasSubtitle"] = False
                        self.DownloadPlace += 1
                        self.ControlDownloadOne()

	#数据连接部分：接收数据，存储在文件里
    def DataLinkReceive(self):		
        '''
        描述：处理服务器的控制连接回复
        参数：回复内容
        返回：无
        '''	
        self.DataSocket, Address = self.ListenSocket.accept()
        self.DataSocket.settimeout(0.5)
        print("Accepted the link of ", Address)
        TheData = self.DataSocket.recv(Constants.DATA_PACKET_SIZE)
        self.WriteFile(TheData)
        print("Dropped the link of ", Address)
        self.CloseDataPort()
        if self.DownloadPlace % 2 == 0:
            self.PlayList[round(self.DownloadPlace / 2)]["WhetherHasSubtitle"] = True
        self.DownloadPlace += 1
        self.ControlDownloadOne()

					
    def WriteFile(self, TheData):
        '''
        描述：写入文件
        参数：数据内容
        返回：无
        '''	
        TheCacheName = self.GetCacheFileName()
        File = open(TheCacheName, "ab")
        File.write(TheData)
        File.close()

    #获取和下载全部文件
    def SetPlayList(self, TheReply):
        '''
        描述：解析文件列表和数据
        参数：待解析的数据内容
        返回：无
        '''	
        Lines = TheReply.split('\n')
        TheFileList = Lines[3].split()
        TheItem = {}
        for i in range(len(TheFileList)):
            if i % 3 == 0:
                TheItem["FileName"] = TheFileList[i]
            elif i % 3 == 1:
                TheItem["TotalFrameNumber"] = int(TheFileList[i])
                TheItem["CurrentFrameNumber"] = 0
            else:
                TheItem["FramePerSecond"] = int(TheFileList[i])
                TheItem["TotalTime"] = self.GetPlayTime(TheItem["TotalFrameNumber"], int(TheFileList[i]))
                TheItem["CurrentTime"] = self.GetPlayTime(0, int(TheFileList[i]))
                self.PlayList.append(TheItem)
                TheItem = {}
        #print(self.PlayList)

    def GetAllFiles(self):
        '''
        描述：控制从服务器下载全部文件
        参数：数据内容
        返回：无
        '''
        for item in self.PlayList:
            ThePictureName = self.GetDownloadFileName(item["FileName"], True)
            TheSubtitleName = self.GetDownloadFileName(item["FileName"], False)
            self.DownloadList.append(TheSubtitleName)
            self.DownloadList.append(ThePictureName)
        print(self.DownloadList)
        self.ControlDownloadOne()

    def ControlDownloadOne(self):
        '''
        描述：控制从服务器下载一个文件
        参数：数据内容
        返回：无
        '''
        time.sleep(0.01)
        if self.DownloadPlace >= len(self.DownloadList):
            self.TearDownLink()
            return
        self.FileName = self.DownloadList[self.DownloadPlace]
        self.BeforeDownload()

    #基本操作函数，比如随机生成端口，生成完整文件名
    def InitDir(self):
        if os.path.exists(self.SaveDir) == False:
            os.mkdir(self.SaveDir)
        if os.path.exists(self.SaveDir + '/' + str(self.Session)) == False:
            os.mkdir(self.SaveDir + '/' + str(self.Session))

    def GenerateRandomPort(self):	
        '''
        描述：生成随机的自身数据端口
        参数：无
        返回：一个随机数port
        '''
        ThePort = random.randint(10001, 65535)
        return ThePort

    def GetCacheFileName(self):
        '''
        描述：根据session，前缀等生成图片，字幕文件名
        参数：无
        返回：文件名
        '''
        TheFileName = self.SaveDir + '/' + str(self.Session) + '/' + self.FileName
        return TheFileName

    def GetDownloadFileName(self, TheFileName, Type):
        '''
        描述：生成待下载的图片，字幕文件名
        参数：无
        返回：文件名
        '''
        Back = ""
        if Type == True:
            Back = self.PictureBack
        else:
            Back = self.SubtitleBack
        TheFileNameDownload = TheFileName[0:-4] + Back
        return TheFileNameDownload
    
    def GetIconFileName(self, TheFileName):
        '''
        描述：缩略图片的文件名
        参数：无
        返回：文件名
        '''
        return self.SaveDir + '/' + str(self.Session) + '/' + TheFileName[0:-4] + self.PictureBack

    def GetSubtitleFileName(self, TheFileName):
        '''
        描述：字幕文件的文件名
        参数：无
        返回：文件名
        '''
        return self.SaveDir + '/' + str(self.Session) + '/' + TheFileName[0:-4] + self.SubtitleBack

    def GetPlayTime(self, TheFrameNumber, TheFramePerSecond):
        '''
        描述：根据播放帧数计算播放时间
        参数：帧数,帧率
        返回：字符串，代表时间
        '''
        TotalSecond = round(TheFrameNumber / TheFramePerSecond)
        TheHour = floor(TotalSecond / 3600)
        TheMinute = floor((TotalSecond - TheHour * 3600) / 60)
        TheSecond = TotalSecond % 60
        TheString = str(TheHour) + ":" + str(TheMinute).zfill(2) + ":" + str(TheSecond).zfill(2)
        return TheString

    #GUI相关函数
    def ConnectSignalAndSlot(self):
        '''
        描述：连接各种信号和槽
        参数：无
        返回：无
        '''
        self.LoginWindow.LoginButton.clicked.connect(self.Initialize)
        self.InitializeFinished.connect(self.InitializeGUI)
        self.MainWindow.VideoList.itemClicked.connect(self.ShowDetail)
        self.MainWindow.SearchButton.clicked.connect(self.SearchInPlayList)
        self.MainWindow.VideoList.setContextMenuPolicy(Qt.CustomContextMenu)######允许右键产生子菜单
        self.MainWindow.VideoList.customContextMenuRequested.connect(self.GenerateMenu)   ####右键菜单

    def InitializeGUI(self):
        '''
        描述：初始化gui，打开mainwindow和加载播放列表
        参数：无
        返回：无
        '''
        #self.MainWindow.show()
        self.InitializePlayList()
        self.MainWindow.show()
        self.LoginWindow.close()

    def InitializePlayList(self):
        '''
        描述：加载播放列表
        参数：无
        返回：无
        '''
        self.MainWindow.VideoList.setViewMode(QListView.IconMode)
        self.MainWindow.VideoList.setIconSize(QSize(150, 150))
        self.MainWindow.VideoList.setSpacing(12)
        for item in self.PlayList:
            NewItem = QListWidgetItem()
            TheIconName = self.GetIconFileName(item["FileName"])
            NewItem.setIcon(QIcon(TheIconName))
            NewItem.setText(item["FileName"])
            self.MainWindow.VideoList.addItem(NewItem)

    def SearchInPlayList(self):
        '''
        描述：处理播放列表搜索
        参数：无
        返回：无
        '''
        self.MainWindow.VideoList.clear()
        TheText = self.MainWindow.SearchText.text()
        if TheText == '':
            for item in self.PlayList:
                NewItem = QListWidgetItem()
                TheIconName = self.GetIconFileName(item["FileName"])
                NewItem.setIcon(QIcon(TheIconName))
                NewItem.setText(item["FileName"])
                self.MainWindow.VideoList.addItem(NewItem)
        else:
            for item in self.PlayList:
                if re.search(TheText, item["FileName"]):
                    NewItem = QListWidgetItem()
                    TheIconName = self.GetIconFileName(item["FileName"])
                    NewItem.setIcon(QIcon(TheIconName))
                    NewItem.setText(item["FileName"])
                    self.MainWindow.VideoList.addItem(NewItem)

    def ShowDetail(self, item):
        '''
        描述：点击某一项后，下面框显示细节
        参数：无
        返回：无
        '''
        TheItem = None
        for one in self.PlayList:
            if item.text() == one["FileName"]:
                TheItem = one
                break
        if TheItem["WhetherHasSubtitle"] == True:
            TheSubtitle = "是"
        else:
            TheSubtitle = "否"
        TheShowDetail = "视频名称： " + TheItem["FileName"] + '\n'\
        + "总时长： " + TheItem["TotalTime"] + '\n'\
        + "当前播放时长： " + TheItem["CurrentTime"] + '\n'\
        + "是否有字幕： " + TheSubtitle
        self.MainWindow.ShowText.setPlainText(TheShowDetail)

    def GenerateMenu(self, Position):
        '''
        描述：选择元素后右键生成菜单
        参数：位置
        返回：无
        '''
        item = self.MainWindow.VideoList.itemAt(Position) 
        try:
            DirName = item.text()
        except:
            return
        TheItem = None
        for one in self.PlayList:
            if DirName == one["FileName"]:
                TheItem = one
                break

        Menu = QMenu()
        Item0 = Menu.addAction("从头开始播放")
        Item1 = Menu.addAction("继续播放")
        Action = Menu.exec_(self.MainWindow.VideoList.mapToGlobal(Position))
        if Action == Item0:
            self.PlayVideo(TheItem, 0)
            #threading.Thread(target = self.PlayVideo, \
            #args = (TheItem, 0)).start()
        elif Action == Item1:
            self.PlayVideo(TheItem, TheItem["CurrentFrameNumber"])
            #threading.Thread(target = self.PlayVideo, \
            #args = (TheItem, TheItem["CurrentFrameNumber"])).start()
        else:
            donothing = True
                

    def PlayVideo(self, PlayListItem, StartPlace):
        '''
        描述：控制视频播放
        参数：位置
        返回：无
        '''
        print("Started playing a video of ", PlayListItem["FileName"])
        time.sleep(0.1)
        self.MainWindow.setVisible(False)
        Root = None
        Root = Tk()
        TheClient = PlayClient(Root, self.ServerIP, Constants.RTP_SERVER_CONTROL_PORT,\
        PlayListItem["FileName"], StartPlace, self.Session, PlayListItem["WhetherHasSubtitle"])
        Root.mainloop()
        try:
            Root.destroy()
        except:
            Root = None
        PlayListItem["CurrentFrameNumber"] = Constants.CurrentFrameBuffer
        PlayListItem["CurrentTime"] = self.GetPlayTime(PlayListItem["CurrentFrameNumber"], PlayListItem["FramePerSecond"])
        self.MainWindow.setVisible(True)
        print(Constants.CurrentFrameBuffer)
        print("Stop playing a video of ", PlayListItem["FileName"])


if __name__ == '__main__':
    app = QApplication(sys.argv)
    TheClient = MainClient()
    sys.exit(app.exec_())


