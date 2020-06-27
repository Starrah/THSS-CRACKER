import socket
import sys
import os
import math
from PyQt5.QtWidgets import QApplication, QMainWindow, QInputDialog, QFileDialog, QPushButton, QLineEdit, QTableWidgetItem, QMessageBox, QProgressBar, QMenu, QAbstractItemView
from PyQt5.QtCore import QObject, Qt, QThread, pyqtSignal
from PyQt5.uic import loadUi
import time
import _thread

class Client:
    ControlConnection = socket.socket()         
    DataConnection = socket.socket()
    ListenConnection = socket.socket()
    StartPlace = 0
    ConnectionType = "No"
    WhetherConnected = False
    WhetherTransporting = False
    ClientIP = "127.0.0.1"
    ServerIP = "127.0.0.1"
    #服务器端口
    ControlPort = 21
    DataPort = -1
    Username = "anonymous"
    Password = "114514"
    CurrentDir = "/"
    DownloadQueue = []
    UploadQueue = []
    def __init__(self):
        '''
        描述：构造函数
        参数：无
        '''
        # 获取本机ip
        hostname = socket.gethostname()
        self.ClientIP = socket.gethostbyname(hostname)
        #初始化ui
        self.LoginWindow = loadUi('LoginWindow.ui')
        self.MainWindow = loadUi("mainwindow.ui")
        self.ConnectSignalAndSlot()
        self.LoginWindow.show()

    
    def ConnectSignalAndSlot(self):
        '''
        描述：连接各种信号和槽
        参数：无
        返回：无
        '''

        self.MainWindow.FileListTable.setSelectionBehavior(QAbstractItemView.SelectRows) ###设置一次选中一行
        self.MainWindow.FileListTable.setEditTriggers(QAbstractItemView.NoEditTriggers) ###设置表格禁止编辑
        self.MainWindow.FileListTable.setContextMenuPolicy(Qt.CustomContextMenu)######允许右键产生子菜单
        self.MainWindow.FileListTable.customContextMenuRequested.connect(self.GenerateMenu)   ####右键菜单
        
        self.LoginWindow.LoginButton.clicked.connect(self.InitAndLogin)
        self.MainWindow.RefreshButton.clicked.connect(self.RefreshPath)
        self.MainWindow.RefreshButton.clicked.connect(self.GetFileList)
        self.MainWindow.QuitButton.clicked.connect(self.Disconnect)
        self.MainWindow.GoToButton.clicked.connect(self.ChangeDir)
        self.MainWindow.NewButton.clicked.connect(self.MakeDir)
        self.MainWindow.BackButton.clicked.connect(self.GoBack)
        self.MainWindow.RootButton.clicked.connect(self.ChangeToRoot)
        self.MainWindow.UploadButton.clicked.connect(self.ShowUploadDialog)

        self.MainWindow.DownloadTable.setEditTriggers(QAbstractItemView.NoEditTriggers)
        self.MainWindow.UploadTable.setEditTriggers(QAbstractItemView.NoEditTriggers)


    def JudgeStatusCode(self, Return):
        '''
        描述：判断请求码
        参数：response
        返回：成功（<400)True，否则False
        '''
        try:
            StatusCode = int(Return.split(' ')[0])
            if StatusCode > 0 and StatusCode < 400:
                return True
            else:
                return False
        except:
            return False

    def InitAndLogin(self):
        '''
        描述：连接socket与登录
        参数：无
        连接信号：gui的login信号
        '''
        Success = True
        Reason = ''
        if Success == True:
            try:
                self.ServerIP = self.LoginWindow.IPText.toPlainText() 
                self.ControlPort = int(self.LoginWindow.PortText.toPlainText())
                self.Username = self.LoginWindow.UserText.toPlainText()
                self.Password = self.LoginWindow.PassText.toPlainText()
            except:
                Success = False
                Reason = "Invalid Port"
        #if self.Username != "anonymous":
        #    Success = False
        #    Reason = "User error, must be anonymous"
        if Success == True:
            try:
                print(self.ServerIP, self.ControlPort)
                self.ControlConnection.connect((self.ServerIP, self.ControlPort))
            except:
                Reason = "Cannot connect successfully"
                Success = False
        if Success == True:
            Return = self.ControlConnection.recv(8192).decode('UTF-8','strict')
            print(Return)
            if self.JudgeStatusCode(Return) == False:
                Success = False
                Reason = "Cannot connect successfully"

        if Success == True:
            SendMessage = "USER " + self.Username[ : ] +'\r\n'
            self.ControlConnection.send(str.encode(SendMessage))
            Return = self.ControlConnection.recv(8192).decode('UTF-8','strict')
            if self.JudgeStatusCode(Return) == False:
                Success = False
                Reason = "User error, must be anonymous"
        if Success == True:
            SendMessage = "PASS " + self.Password[ : ] +'\r\n'
            self.ControlConnection.send(str.encode(SendMessage))
            Return = self.ControlConnection.recv(8192).decode('UTF-8','strict')
            if self.JudgeStatusCode(Return) == False:
                Success = False
                Reason = "Password error, must be anonymous"
        if Success == True:
            self.LoginWindow.close()
            self.MainWindow.show()
            self.RefreshPath()
            self.GetFileList()
        else:
            QMessageBox.warning(self.LoginWindow,'Logging Fail',Reason,QMessageBox.Yes)
            self.LoginWindow.close()

    def Disconnect(self):
        '''
        描述：退出登录，QUIT
        参数：无
        返回：无
        '''
        SendMessage = "QUIT\r\n"
        self.ControlConnection.send(str.encode(SendMessage))
        Return = self.ControlConnection.recv(8192).decode('UTF-8','strict')
        Client.ConnectionType = "No"
        Client.WhetherConnected = False
        Client.WhetherTransporting = False
        #self.LoginWindow.show()
        self.MainWindow.close()

    def GenerateMenu(self, Position):
        '''
        描述：选择元素后右键生成菜单
        参数：位置
        返回：无
        '''
        Row = self.MainWindow.FileListTable.currentRow()
        try:
            DirName = str(self.MainWindow.FileListTable.takeItem(Row, 0).text())
            Size = int(self.MainWindow.FileListTable.takeItem(Row, 3).text())
        except:
            return
        Itemaa = QTableWidgetItem(DirName)
        Itembb = QTableWidgetItem(str(Size))
        self.MainWindow.FileListTable.setItem(Row, 0, Itemaa)
        self.MainWindow.FileListTable.setItem(Row, 3, Itembb)

        Menu = QMenu()
        Item0 = Menu.addAction("GoInto")
        Item1 = Menu.addAction("Rename")
        Item2 = Menu.addAction("Delete")
        Item3 = Menu.addAction("DownLoad")
        Action = Menu.exec_(self.MainWindow.FileListTable.mapToGlobal(Position))
        if Action == Item0:
            self.GoIntoDir(DirName)
            self.GetFileList()
        elif Action == Item1:
            self.RenameDir(DirName)
            self.GetFileList()
        elif Action == Item2:
            self.DeleteDir(DirName)
            self.GetFileList()
        elif Action == Item3:
            self.ControlFileDownload(DirName, Size)
        else:
            iii = 0
   
    def JudgeDirValid(self, Name):
        '''
        描述：判断一个文件夹名是否合法
        参数：名字
        返回：True合法False非法
        '''
        if len(Name) == 0:
            return False
        for item in Name:
            if((item < '0' or item > '9') and (item < 'a' or item > 'z') and (item < 'A' or item > 'Z') and (item != '_')):
                return False
        return True
    
    def JudgePathValid(self, Name):
        '''
        描述：判断一个目录名是否合法
        参数：名字
        返回：True合法False非法
        '''
        if len(Name) == 0:
            return False
        for item in Name:
            if(item == '\n' or item == '\r' or item == ' '):
                return False
        return True

    def MakeDir(self):
        '''
        描述：新建目录,MKD
        参数：无
        返回：无
        '''
        DirName, _ = QInputDialog.getText(self.MainWindow, "Please input the Dirname", "Only English Characters, numbers and _ are valid", QLineEdit.Normal, "")
        if(self.JudgeDirValid(DirName) == True):
            SendMessage = "MKD " + DirName + '\r\n'
            self.ControlConnection.send(str.encode(SendMessage))
            Return = self.ControlConnection.recv(8192).decode('UTF-8','strict')
            if(self.JudgeStatusCode(Return) == True):
                self.GetFileList()
            else:
                QMessageBox.warning(self.MainWindow,'Fail',"Make Directory Fail",QMessageBox.Yes)
        else:
            QMessageBox.warning(self.MainWindow,'Fail',"Invalid Dirname",QMessageBox.Yes)

    def GoBack(self):
        '''
        描述：回到上一级
        参数：无
        返回：无
        '''
        if self.CurrentDir == '/':
            QMessageBox.warning(self.MainWindow,'Fail',"Now it is the root directory",QMessageBox.Yes)
        else:
            if self.CurrentDir[-1] == '/':
                for i in range(len(self.CurrentDir) - 1):
                    j = len(self.CurrentDir) - 2 - i
                    if self.CurrentDir[j] == '/':
                        break
            else:
                for i in range(len(self.CurrentDir)):
                    j = len(self.CurrentDir) - 1 - i
                    if self.CurrentDir[j] == '/':
                        break
            NewDir = self.CurrentDir[ : j + 1]
            if(self.JudgePathValid(NewDir) == True):
                SendMessage = "CWD " + NewDir +'\r\n'
                self.ControlConnection.send(str.encode(SendMessage))
                Return = self.ControlConnection.recv(8192).decode('UTF-8','strict')
                if(self.JudgeStatusCode(Return) == True):
                    self.RefreshPath()
                    self.GetFileList()
                else:
                    QMessageBox.warning(self.MainWindow,'Fail',"Change Working Directory Fail",QMessageBox.Yes)
            else:
                QMessageBox.warning(self.MainWindow,'Fail',"Invalid Directory, cannot contain blank",QMessageBox.Yes)

    def ChangeDir(self):
        '''
        描述：修改目录为输入目录，CWD
        参数：无
        返回：无
        '''
        TheDir = self.MainWindow.PathText.toPlainText()
        if(self.JudgePathValid(TheDir) == True):
            SendMessage = "CWD " + TheDir +'\r\n'
            self.ControlConnection.send(str.encode(SendMessage))
            Return = self.ControlConnection.recv(8192).decode('UTF-8','strict')
            if(self.JudgeStatusCode(Return) == True):
                self.RefreshPath()
                self.GetFileList()
            else:
                QMessageBox.warning(self.MainWindow,'Fail',"Change Working Directory Fail",QMessageBox.Yes)
        else:
            QMessageBox.warning(self.MainWindow,'Fail',"Invalid Directory, cannot contain blank",QMessageBox.Yes)

    def GoIntoDir(self, Dirname):
        '''
        描述：进入目录
        参数：目录名
        返回：无
        '''
        if(self.JudgePathValid(Dirname) == True):
            SendMessage = "CWD " + Dirname +'\r\n'
            self.ControlConnection.send(str.encode(SendMessage))
            Return = self.ControlConnection.recv(8192).decode('UTF-8','strict')
            if(self.JudgeStatusCode(Return) == True):
                self.RefreshPath()
                self.GetFileList()
            else:
                QMessageBox.warning(self.MainWindow,'Fail',"Change Working Directory Fail",QMessageBox.Yes)
        else:
            QMessageBox.warning(self.MainWindow,'Fail',"Invalid Directory, cannot contain blank",QMessageBox.Yes)

    def ChangeToRoot(self):
        '''
        描述：修改目录为根目录，CDUP
        参数：无
        返回：无
        '''
        SendMessage = "CDUP\r\n"
        self.ControlConnection.send(str.encode(SendMessage))
        Return = self.ControlConnection.recv(8192).decode('UTF-8','strict')
        if(self.JudgeStatusCode(Return) != True):
            QMessageBox.warning(self.MainWindow,'Fail',"Change Working Directory Fail",QMessageBox.Yes)
        else:
            self.RefreshPath()
            self.GetFileList()

    def DeleteDir(self, FileName):
        '''
        描述：删除目录，RMD
        参数：目录名
        返回：无
        '''
        SendMessage = "RMD "+ FileName + '\r\n'
        self.ControlConnection.send(str.encode(SendMessage))
        Return = self.ControlConnection.recv(8192).decode('UTF-8','strict')
        if(self.JudgeStatusCode(Return) != True):
            QMessageBox.warning(self.MainWindow,'Fail',"Delete Working Directory Fail, the directory may be not empty or you lack the permission",QMessageBox.Yes)
        else:
            self.RefreshPath()
            self.GetFileList()
    
    def RenameDir(self, FileName):
        '''
        描述：重命名文件，RNFR+RNTO
        参数：文件名
        返回：无
        '''
        DirName, _ = QInputDialog.getText(self.MainWindow, "Please input the New Name", "You can change the dir of the file as well using /, but no blank is permitted.", QLineEdit.Normal, "")
        if(self.JudgePathValid(DirName) == True):
            SendMessage = "RNFR " + FileName + '\r\n'
            self.ControlConnection.send(str.encode(SendMessage))
            Return = self.ControlConnection.recv(8192).decode('UTF-8','strict')
            if(self.JudgeStatusCode(Return) == True):
                SendMessage = "RNTO " + DirName + '\r\n'
                self.ControlConnection.send(str.encode(SendMessage))
                Return = self.ControlConnection.recv(8192).decode('UTF-8','strict')
                if(self.JudgeStatusCode(Return) == True):
                    self.GetFileList()
                else:
                    QMessageBox.warning(self.MainWindow,'Fail',"Rename Fail",QMessageBox.Yes)
            else:
                QMessageBox.warning(self.MainWindow,'Fail',"Rename Fail",QMessageBox.Yes)
        else:
            QMessageBox.warning(self.MainWindow,'Fail',"Invalid Name",QMessageBox.Yes)

    def InitPasvConnection(self, IPPortList):
        '''
        描述：初始化数据连接(PASV)
        参数：IP和Port构成的list
        返回：成功true失败false
        '''
        self.ListenConnection.close()
        self.ListenConnection = None
        self.ListenConnection = socket.socket()
        self.DataConnection.close()
        self.DataConnection = None
        self.DataConnection = socket.socket() 
        print(IPPortList)
        Host = IPPortList[0] + '.' + IPPortList[1] + '.' + IPPortList[2] + '.' + IPPortList[3]
        Port = int(IPPortList[4]) * 256 + int(IPPortList[5])
        #print(Port)
        self.DataPort = Port
        return True

    def Connect(self):
        '''
        描述：连接PASV
        参数：无
        返回：成功还是失败
        '''
        self.ControlConnection.send(str.encode("PASV\r\n"))
        Return = self.ControlConnection.recv(8192).decode('UTF-8','strict')
        print(Return)
        #sys.stdout.write(Return)
        if(self.JudgeStatusCode(Return) == True):
            if(Return[-4] == ')'):
                IPPortList = Return[27:-4].split(',')
            else:
                IPPortList = Return[27:-3].split(',')
            print(IPPortList)
            if self.InitPasvConnection(IPPortList) == True:
                Client.ConnectionType = "PASV"
                Client.WhetherConnected = False
                return True
        return False

    def RefreshPath(self):
        '''
        描述：刷新路径显示，PWD
        参数：无
        返回：无
        '''
        SendMessage = "PWD\r\n"
        self.ControlConnection.send(str.encode(SendMessage))
        Return = self.ControlConnection.recv(8192).decode('UTF-8','strict')
        CurrentDir = Return[0:-2].split(' ')[1]
        self.CurrentDir = CurrentDir[1:-1]
        self.MainWindow.PathText.setPlainText(self.CurrentDir)

    def GetFileList(self):
        '''
        描述：获取文件列表并显示，CDUP
        参数：无
        返回：无
        '''
        self.MainWindow.ServerText.setPlainText(self.ServerIP + ":" + str(self.ControlPort))
        self.MainWindow.UserText.setPlainText(self.Username)
        if Client.WhetherTransporting == True:
            QMessageBox.warning(self.MainWindow,'Fail',"There is already a file downloading or uploading, please finish or stop it first",QMessageBox.Yes)
            return
        if self.Connect() != True:
            QMessageBox.warning(self.MainWindow,'Fail',"Connection Failure",QMessageBox.Yes)
            return
        SendMessage = "LIST\r\n"
        self.ControlConnection.send(str.encode(SendMessage))
        if(Client.WhetherConnected != True):
            self.DataConnection.connect((self.ServerIP, self.DataPort))
            Client.WhetherConnected = True
        Return = self.ControlConnection.recv(8192).decode('UTF-8','strict')
        if(self.JudgeStatusCode(Return) == False):
            return
        ReceiveData = self.DataConnection.recv(8192).decode('UTF-8','strict')
        Return = self.ControlConnection.recv(8192).decode('UTF-8','strict')
        print(ReceiveData)
        ReceiveList = ReceiveData.split('\n')
        FileInfoTotal = []
        for i in range(len(ReceiveList)):
            #if(i == 0):
            #    continue
            item = ReceiveList[i]
            ItemList = item.split()
            #print(ItemList)
            FileInfo = []
            try:
                size = int(ItemList[-5])
                FileInfo.append(ItemList[-1])
                if(ItemList[0][0] == '-'):
                    FileInfo.append("File")
                elif(ItemList[0][0] == 'd'):
                    FileInfo.append("Directory")
                else:
                    FileInfo.append("Others")
                FileInfo.append(ItemList[-4] + ItemList[-3] + ' ' + ItemList[-2])
                FileInfo.append(ItemList[-5])
            except:
                iii = 0
            #print(FileInfo)
            if len(FileInfo) == 4:
                FileInfoTotal.append(FileInfo)
        self.MainWindow.FileListTable.setRowCount(len(FileInfoTotal))
        for i in range(len(FileInfoTotal)):
            for j in range(4):
                Item = QTableWidgetItem(FileInfoTotal[i][j])
                self.MainWindow.FileListTable.setItem(i, j, Item)

    def InsertIntoDownloadQueue(self, FileName, Size):
        '''
        描述：将下载任务加入队列
        参数：文件名（部分）,总大小
        返回：无
        '''
        if(self.CurrentDir[-1] == '/'):
            FullFileName = self.CurrentDir[:] + FileName[:]
        else:
            FullFileName = self.CurrentDir[:] + '/' + FileName[:] 
        WhetherSameName = False
        for i in range(len(self.DownloadQueue)):
            try:
                item = self.DownloadQueue[i]
                if item["FileName"] == FileName:
                    WhetherSameName = True
                    break
            except:
                iii = 0
        if(WhetherSameName == True):
            self.DownloadQueue[i]["FullFileName"] = FullFileName
            self.DownloadQueue[i]["CurrentSize"] = 0
            self.DownloadQueue[i]["TotalSize"] = Size
            self.DownloadQueue[i]["Status"] = "Continue" 
        else:
            NewItem = {}
            NewItem["FileName"] = FileName
            NewItem["FullFileName"] = FullFileName
            NewItem["CurrentSize"] = 0
            NewItem["TotalSize"] = Size
            NewItem["Status"] = "Continue"
            self.DownloadQueue.append(NewItem)
        self.RefreshDownloadMenu()

    def InsertIntoUploadQueue(self, FileName, Size):
        '''
        描述：将上传任务加入队列
        参数：文件名(完整）,总大小
        返回：无
        '''
        PartFileName = FileName.split("/")[-1]
        WhetherSameName = False
        for i in range(len(self.UploadQueue)):
            try:
                item = self.UploadQueue[i]
                if item["FileName"] == PartFileName:
                    WhetherSameName = True
                    break
            except:
                iii = 0
        if(WhetherSameName == True):
            self.UploadQueue[i]["FileName"] = PartFileName
            self.UploadQueue[i]["CurrentSize"] = 0
            self.UploadQueue[i]["TotalSize"] = Size
            self.UploadQueue[i]["Status"] = "Continue" 
        else:
            NewItem = {}
            NewItem["FileName"] = PartFileName
            NewItem["FullFileName"] = FileName
            NewItem["CurrentSize"] = 0
            NewItem["TotalSize"] = Size
            NewItem["Status"] = "Continue"
            self.UploadQueue.append(NewItem)      
        self.RefreshUploadMenu()

    def RemoveFromDownloadQueue(self, FileName):
        '''
        描述：将下载任务删除出队列
        参数：文件名（部分）
        返回：无
        '''
        for i in range(len(self.DownloadQueue)):
            item = self.DownloadQueue[i]
            if item["FileName"] == FileName:
                self.DownloadQueue.remove(item)
                break
        self.RefreshDownloadMenu()

    def RemoveFromUploadQueue(self, FileName):
        '''
        描述：将上传任务删除出队列
        参数：文件名（部分）
        返回：无
        '''
        for i in range(len(self.UploadQueue)):
            item = self.UploadQueue[i]
            if item["FileName"] == FileName:
                self.UploadQueue.remove(item)
                break
        self.RefreshUploadMenu()

    def RefreshDownloadMenu(self):
        '''
        描述：刷新下载显示
        参数：无
        返回：无
        '''
        self.MainWindow.DownloadTable.setRowCount(len(self.DownloadQueue))
        for i in range(len(self.DownloadQueue)):
            item = self.DownloadQueue[i]
            NameItem = QTableWidgetItem(item["FileName"])
            self.MainWindow.DownloadTable.setItem(i, 0, NameItem)
            StopButton = QPushButton('STOP')
            StopButton.clicked.connect(self.HandleDownloadButtons)
            self.MainWindow.DownloadTable.setCellWidget(i, 1, StopButton)
            if item["Status"] == "Continue":
                PauseButton = QPushButton('PAUSE', self.MainWindow)
            else:
                PauseButton = QPushButton('RESUME', self.MainWindow)
            PauseButton.clicked.connect(self.HandleDownloadButtons)
            self.MainWindow.DownloadTable.setCellWidget(i, 2, PauseButton)
            ProcessBar = QProgressBar(self.MainWindow)
            Process = int(round((100 * item["CurrentSize"]) / item["TotalSize"]))
            ProcessBar.setValue(Process)
            self.MainWindow.DownloadTable.setCellWidget(i, 3, ProcessBar)

    def RefreshUploadMenu(self):
        '''
        描述：刷新上传显示
        参数：无
        返回：无
        '''
        self.MainWindow.UploadTable.setRowCount(len(self.UploadQueue))
        for i in range(len(self.UploadQueue)):
            item = self.UploadQueue[i]
            NameItem = QTableWidgetItem(item["FileName"])
            self.MainWindow.UploadTable.setItem(i, 0, NameItem)
            StopButton = QPushButton('STOP')
            StopButton.clicked.connect(self.HandleUploadButtons)
            self.MainWindow.UploadTable.setCellWidget(i, 1, StopButton)
            if item["Status"] == "Continue":
                PauseButton = QPushButton('PAUSE', self.MainWindow)
            else:
                PauseButton = QPushButton('RESUME', self.MainWindow)
            PauseButton.clicked.connect(self.HandleUploadButtons)
            self.MainWindow.UploadTable.setCellWidget(i, 2, PauseButton)
            ProcessBar = QProgressBar(self.MainWindow)
            Process = int(round((100 * item["CurrentSize"]) / item["TotalSize"]))
            ProcessBar.setValue(Process)
            self.MainWindow.UploadTable.setCellWidget(i, 3, ProcessBar)

    def AddProcessDownload(self, FileName, Length):
        '''
        描述：修改进度和进度条
        参数：文件名,大小
        返回：无
        '''
        for i in range(len(self.DownloadQueue)):
            item = self.DownloadQueue[i]
            if item["FileName"] == FileName:
                item["CurrentSize"] = item["CurrentSize"] + Length
                ProcessBar = QProgressBar(self.MainWindow)
                if(item["TotalSize"] != 0):
                    Process = int(round((100 * item["CurrentSize"]) / item["TotalSize"]))
                else:
                    Process = 100                
                ProcessBar.setValue(Process)
                self.MainWindow.DownloadTable.setCellWidget(i, 3, ProcessBar)
                break

    def AddProcessUpload(self, FileName, Length):
        '''
        描述：修改进度和进度条
        参数：文件名,大小
        返回：无
        '''
        PartFileName = FileName.split('/')[-1]
        for i in range(len(self.UploadQueue)):
            item = self.UploadQueue[i]
            if item["FileName"] == PartFileName:
                item["CurrentSize"] = item["CurrentSize"] + Length
                ProcessBar = QProgressBar(self.MainWindow)
                if(item["TotalSize"] != 0):
                    Process = int(round((100 * item["CurrentSize"]) / item["TotalSize"]))
                else:
                    Process = 100
                #print(item["CurrentSize"], item["TotalSize"], Process)
                ProcessBar.setValue(Process)
                self.MainWindow.UploadTable.setCellWidget(i, 3, ProcessBar)
                break

    def SetDownloadPause(self, FileName):
        '''
        描述：修改当前状态为pause
        参数：文件名,大小
        返回：无
        '''
        for i in range(len(self.DownloadQueue)):
            item = self.DownloadQueue[i]
            if item["FileName"] == FileName:
                item["Status"] = "Pause"
                break
        self.RefreshDownloadMenu()

    def SetUploadPause(self, FileName):
        '''
        描述：修改当前状态为pause
        参数：文件名,大小
        返回：无
        '''
        for i in range(len(self.UploadQueue)):
            item = self.UploadQueue[i]
            if item["FileName"] == FileName:
                item["Status"] = "Pause"
                break
        self.RefreshUploadMenu()

    def SetDownloadContinue(self, FileName):
        '''
        描述：修改当前状态为continue
        参数：文件名,大小
        返回：无
        '''
        for i in range(len(self.DownloadQueue)):
            item = self.DownloadQueue[i]
            if item["FileName"] == FileName:
                item["Status"] = "Continue"
                break
        self.RefreshDownloadMenu()

    def SetUploadContinue(self, FileName):
        '''
        描述：修改当前状态为continue
        参数：文件名,大小
        返回：无
        '''
        for i in range(len(self.UploadQueue)):
            item = self.UploadQueue[i]
            if item["FileName"] == FileName:
                item["Status"] = "Continue"
                break
        self.RefreshUploadMenu()

    def DownloadPause(self, FileName, WhetherFinished):
        '''
        描述：控制文件下载暂停（完成和暂停，结束都是）
        参数：文件名,总大小
        返回：无
        '''
        self.DataConnection.close()
        self.DataConnection = None
        self.DataConnection = socket.socket()    
        Client.ConnectionType = "NO"
        Client.WhetherConnected = False
        Client.WhetherTransporting = False
        
        Success = True
        if(WhetherFinished == True):
            Return = self.ControlConnection.recv(8192).decode('UTF-8','strict')
            if self.JudgeStatusCode(Return) == False:
                Success = False
            if Success == False:
                QMessageBox.warning(self.MainWindow,'Fail',"Download Failure",QMessageBox.Yes)
            else:
                QMessageBox.information(self.MainWindow,'Success',"Download Success",QMessageBox.Yes)
            self.RemoveFromDownloadQueue(FileName)
        else:
            Return = self.ControlConnection.recv(8192).decode('UTF-8','strict')
            print(Return)
            self.SetDownloadPause(FileName)
        self.RefreshDownloadMenu()

    def UploadPause(self, FileName, WhetherFinished):
        '''
        描述：控制文件上传暂停（完成和暂停，结束都是）
        参数：文件名,总大小
        返回：无
        '''
        self.DataConnection.close()
        self.DataConnection = None
        self.DataConnection = socket.socket()    
        Client.ConnectionType = "NO"
        Client.WhetherConnected = False
        Client.WhetherTransporting = False
        PartFileName = FileName.split("/")[-1]
        Success = True
        if(WhetherFinished == True):
            Return = self.ControlConnection.recv(8192).decode('UTF-8','strict')
            if self.JudgeStatusCode(Return) == False:
                Success = False
            if Success == False:
                QMessageBox.warning(self.MainWindow,'Fail',"Upload Failure",QMessageBox.Yes)
            else:
                QMessageBox.information(self.MainWindow,'Success',"Upload Success",QMessageBox.Yes)
            self.RemoveFromUploadQueue(PartFileName)
        else:
            Return = self.ControlConnection.recv(8192).decode('UTF-8','strict')
            #print(Return)
            self.SetUploadPause(PartFileName)
        self.RefreshUploadMenu()

    def ControlFileDownload(self, FileName, Size):
        '''
        描述：控制文件下载，RETR
        参数：文件名,总大小
        返回：无
        '''
        if Client.WhetherTransporting == True:
            QMessageBox.warning(self.MainWindow,'Fail',"There is already a file downloading or uploading, please finish or stop it first",QMessageBox.Yes)
            return
        #联网
        if self.Connect() != True:
            QMessageBox.warning(self.MainWindow,'Fail',"Connection Failure",QMessageBox.Yes)
            return

        SendMessage = "RETR " + FileName + "\r\n"
        self.ControlConnection.send(str.encode(SendMessage))
        if Client.WhetherConnected != True:
            self.DataConnection.connect((self.ServerIP, self.DataPort))
            Client.WhetherConnected = True
        Return = self.ControlConnection.recv(8192).decode('UTF-8','strict')
        if self.JudgeStatusCode(Return) == False:
            QMessageBox.warning(self.MainWindow,'Fail',"Download Fail",QMessageBox.Yes)
            return
        
        #加入任务队列，显示
        self.InsertIntoDownloadQueue(FileName, Size)
        Client.WhetherTransporting = True

        #多一个线程下载
        self.DownloadThread = DownloadThread(FileName, 0, self.DataConnection)
        self.DownloadThread.DownloadProcessSignal.connect(self.AddProcessDownload)
        self.DownloadThread.DownloadStopSignal.connect(self.DownloadPause)
        self.DownloadThread.start()

    def ControlFileUpload(self, FileName, Size):
        '''
        描述：控制文件上传，STOR
        参数：文件名,总大小
        返回：无
        '''
        PartFileName = FileName.split('/')[-1]
        if Client.WhetherTransporting == True:
            QMessageBox.warning(self.MainWindow,'Fail',"There is already a file downloading or uploading, please finish or stop it first",QMessageBox.Yes)
            return
        #联网
        if self.Connect() != True:
            QMessageBox.warning(self.MainWindow,'Fail',"Connection Failure",QMessageBox.Yes)
            return

        SendMessage = "STOR " + PartFileName + "\r\n"
        self.ControlConnection.send(str.encode(SendMessage))
        if Client.WhetherConnected != True:
            self.DataConnection.connect((self.ServerIP, self.DataPort))
            Client.WhetherConnected = True
        Return = self.ControlConnection.recv(8192).decode('UTF-8','strict')
        print(Return)
        if self.JudgeStatusCode(Return) == False:
            QMessageBox.warning(self.MainWindow,'Fail',"Upload Fail",QMessageBox.Yes)
            return
        
        #加入任务队列，显示
        self.InsertIntoUploadQueue(FileName, Size)
        Client.WhetherTransporting = True

        #多一个线程下载
        self.UploadThread = UploadThread(FileName, 0, self.DataConnection)
        self.UploadThread.UploadProcessSignal.connect(self.AddProcessUpload)
        self.UploadThread.UploadStopSignal.connect(self.UploadPause)
        self.UploadThread.start()

    def ControlDownloadResume(self, FileName):
        '''
        描述：断点续传
        参数：文件名
        返回：无
        '''
        if Client.WhetherTransporting == True:
            QMessageBox.warning(self.MainWindow,'Fail',"There is already a file downloading or uploading, please finish or stop it first",QMessageBox.Yes)
            return
        #联网
        if self.Connect() != True:
            QMessageBox.warning(self.MainWindow,'Fail',"Connection Failure",QMessageBox.Yes)
            return

        CurrentSize = 0
        FullFileName = ''
        for i in range(len(self.DownloadQueue)):
            item = self.DownloadQueue[i]
            if item["FileName"] == FileName:
                CurrentSize = item["CurrentSize"]
                FullFileName = item["FullFileName"][:]
                break
        
        #REST
        SendMessage = "REST " + str(CurrentSize) + "\r\n"
        self.ControlConnection.send(str.encode(SendMessage))
        Return = self.ControlConnection.recv(8192).decode('UTF-8','strict')
        print(Return)
        if self.JudgeStatusCode(Return) == False:
            QMessageBox.warning(self.MainWindow,'Fail',"Download Fail",QMessageBox.Yes)
            return
        
        #RETR
        SendMessage = "RETR " + FullFileName + "\r\n"
        self.ControlConnection.send(str.encode(SendMessage))
        if Client.WhetherConnected != True:
            print(self.ServerIP, self.DataPort)
            self.DataConnection.connect((self.ServerIP, self.DataPort))
            Client.WhetherConnected = True
        Return = self.ControlConnection.recv(8192).decode('UTF-8','strict')
        print(Return)
        if self.JudgeStatusCode(Return) == False:
            QMessageBox.warning(self.MainWindow,'Fail',"Download Fail",QMessageBox.Yes)
            return

        Client.WhetherTransporting = True
        self.SetDownloadContinue(FileName)

        #多一个线程下载
        self.DownloadThread = DownloadThread(FileName, CurrentSize, self.DataConnection)
        self.DownloadThread.DownloadProcessSignal.connect(self.AddProcessDownload)
        self.DownloadThread.DownloadStopSignal.connect(self.DownloadPause)
        self.DownloadThread.start()

    def ControlUploadResume(self, FileName):
        '''
        描述：断点续传
        参数：文件名
        返回：无
        '''
        if Client.WhetherTransporting == True:
            QMessageBox.warning(self.MainWindow,'Fail',"There is already a file downloading or uploading, please finish or stop it first",QMessageBox.Yes)
            return
        #联网
        if self.Connect() != True:
            QMessageBox.warning(self.MainWindow,'Fail',"Connection Failure",QMessageBox.Yes)
            return

        CurrentSize = 0
        FullFileName = ''
        for i in range(len(self.UploadQueue)):
            item = self.UploadQueue[i]
            if item["FileName"] == FileName:
                CurrentSize = item["CurrentSize"]
                FullFileName = item["FullFileName"][:]
                self.UploadQueue[i]["Status"] = "Continue"
                break
        
        #REST
        SendMessage = "REST " + str(CurrentSize) + "\r\n"
        self.ControlConnection.send(str.encode(SendMessage))
        Return = self.ControlConnection.recv(8192).decode('UTF-8','strict')
        if self.JudgeStatusCode(Return) == False:
            QMessageBox.warning(self.MainWindow,'Fail',"Download Fail",QMessageBox.Yes)
            return
        
        #STOR
        SendMessage = "STOR " + FullFileName + "\r\n"
        self.ControlConnection.send(str.encode(SendMessage))
        if Client.WhetherConnected != True:
            self.DataConnection.connect((self.ServerIP, self.DataPort))
            Client.WhetherConnected = True
        Return = self.ControlConnection.recv(8192).decode('UTF-8','strict')
        if self.JudgeStatusCode(Return) == False:
            QMessageBox.warning(self.MainWindow,'Fail',"Download Fail",QMessageBox.Yes)
            return

        Client.WhetherTransporting = True
        self.SetUploadContinue(FileName)

        #多一个线程上传
        self.UploadThread = UploadThread(FullFileName, CurrentSize, self.DataConnection)
        self.UploadThread.UploadProcessSignal.connect(self.AddProcessUpload)
        self.UploadThread.UploadStopSignal.connect(self.UploadPause)
        self.UploadThread.start()

    def HandleDownloadButtons(self):
        '''
        描述：处理下载table的点击按钮事件
        参数：行，列
        返回：无
        '''
        Row = self.MainWindow.DownloadTable.currentRow()
        Col = self.MainWindow.DownloadTable.currentColumn()
        #print(Row, Col)
        FileName = str(self.MainWindow.DownloadTable.takeItem(Row, 0).text())
        if Col == 1:
            #彻底stop
            if self.DownloadQueue[Row]["Status"] == "Continue":
                Client.WhetherTransporting = False
                time.sleep(0.02)
            self.RemoveFromDownloadQueue(FileName)
        elif Col == 2:
            if self.DownloadQueue[Row]["Status"] == "Continue":
                #pause
                Client.WhetherTransporting = False
            else:
                #continue
                self.ControlDownloadResume(FileName)
        self.RefreshDownloadMenu()
    
    def HandleUploadButtons(self):
        '''
        描述：处理上传table的点击按钮事件
        参数：行，列
        返回：无
        '''
        Row = self.MainWindow.UploadTable.currentRow()
        Col = self.MainWindow.UploadTable.currentColumn()
        #print(Row, Col)
        FileName = str(self.MainWindow.UploadTable.takeItem(Row, 0).text())
        if Col == 1:
            #彻底stop
            if self.UploadQueue[Row]["Status"] == "Continue":
                Client.WhetherTransporting = False
                time.sleep(0.02)
            self.RemoveFromUploadQueue(FileName)
        elif Col == 2:
            if self.UploadQueue[Row]["Status"] == "Continue":
                #pause
                Client.WhetherTransporting = False
            else:
                #continue
                self.ControlUploadResume(FileName)
        self.RefreshUploadMenu()

    def ShowUploadDialog(self):
        FileName,  _ = QFileDialog.getOpenFileName(self.MainWindow, 'Open file', './')
        try:
            Size = os.path.getsize(FileName)
            print(FileName, Size)
        except:
            QMessageBox.warning(self.MainWindow,'Fail',"Invalid FileName",QMessageBox.Yes)
            return

        self.ControlFileUpload(FileName, Size)


class DownloadThread(QThread):
    DownloadProcessSignal = pyqtSignal(str, int)                        #创建信号
    DownloadStopSignal = pyqtSignal(str, bool)
    def __init__(self, FileName, Start, Link):
        super(DownloadThread, self).__init__()
        self.FileName = FileName
        self.Start = Start
        self.Link = Link

    def run(self):
        '''
        描述：多线程接收文件
        参数：文件名,起始位置
        返回：无
        '''

        Success = True
        try:
            File = open(self.FileName, 'rb+')
        except:
            if self.Start == 0:
                File = open(self.FileName, 'wb+')
        try:
            File.seek(self.Start, 0)
        except:
            File.close()
            Success = False

        Finished = True
        if Success == True:
            while(1):
                ReceiveData = self.Link.recv(8192)
                if Client.WhetherTransporting == False:
                    Finished = False
                    break
                if ReceiveData == None or len(ReceiveData) == 0 or (not ReceiveData):
                    Finished = True
                    break
                File.write(ReceiveData)
                self.DownloadProcessSignal.emit(self.FileName, len(ReceiveData))
                time.sleep(0.001)
            File.close()
        self.DownloadStopSignal.emit(self.FileName, Finished)
        print("Download has Stopped")
        self.exit(0)            #关闭线程

class UploadThread(QThread):
    UploadProcessSignal = pyqtSignal(str, int)                        #创建信号
    UploadStopSignal = pyqtSignal(str, bool)
    def __init__(self, FileName, Start, Link):
        super(UploadThread, self).__init__()
        self.FileName = FileName
        self.Start = Start
        self.Link = Link

    def run(self):
        '''
        描述：多线程发送文件
        参数：文件名,起始位置
        返回：无
        '''
        Success = True
        print(self.FileName)
        if Success == True:
            try:
                File = open(self.FileName, 'rb')
            except:
                Success = False
        print("Success = ",Success)
        if Success == True:
            try:
                File.seek(self.Start, 0)
            except:
                File.close()
                Success = False
        print("Success = ",Success)
        Finished = True
        if Success == True:
            while(1):
                #print(Client.WhetherTransporting)
                if Client.WhetherTransporting != True:
                    Finished = False
                    break
                Data = File.read(8192)
                if len(Data) == 0:
                    Finished = True
                    break
                a = self.Link.send(Data)
                #print(a)
                
                self.UploadProcessSignal.emit(self.FileName, len(Data))
                time.sleep(0.001) 
            print(Finished)
            self.UploadStopSignal.emit(self.FileName, Finished)
            File.close()
        print("Upload has Stopped")
        self.exit(0)            #关闭线程


  

if __name__ == "__main__":
    app = QApplication(sys.argv)
    TheClient = Client()
    sys.exit(app.exec_())

