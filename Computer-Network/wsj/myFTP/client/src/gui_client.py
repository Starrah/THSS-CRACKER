import sys,socket,os,threading,subprocess
from PyQt5.QtWidgets import *
from PyQt5 import QtCore, QtGui
from PyQt5.QtCore import Qt

from client import Client
from Ui_mainwindow import Ui_MainWindow
from pathlib import Path

NO_TASK = 0
DOWNLOADING = 1
UPLOADING = 2

def get_type(file_name, access_string):  #获取文件类型
    if access_string[0] == "d":
        return "folder"
    list = file_name.split(".")
    if len(list) == 2:
        return list[1]+" file"
    else:
        return "file"

def translate_size(size):  #转化文件大小为更宜读的格式
    exp = 0
    size = float(size)
    while size >= 1000 and exp < 3:
        exp += 1
        size = size/1000
    size = round(size, 2)
    units = ["B", "KB", "MB", "GB"]
    return '{:g}{}'.format(size, units[exp])

class ClientWindow(Ui_MainWindow):
    '''客户端窗口类'''
    def __init__(self):
        MainWindow = QMainWindow()
        self.setupUi(MainWindow)

        self.client = Client()
        self.has_connected = False  #是否已经建立连接
        self.create_signals_ans_slots()

        self.menubar.setEnabled(False)
        self.statusbar.setEnabled(False)
        self.goButton.setEnabled(False)
        self.remote_path = "/"
        (status, self.local_path) = subprocess.getstatusoutput('pwd')
        self.trans_mode = "PASV"  #默认PASV模式传输
        self.state = NO_TASK        
        self.render_file_list(self.folderWidget_2,1)
        self.navigationBar_2.setText(self.local_path)
        MainWindow.closeEvent = self.closeEvent
        MainWindow.show()
        sys.exit(app.exec_())

    def create_signals_ans_slots(self):
        '''建立窗体中的信号与槽'''

        self.folderWidget.customContextMenuRequested[QtCore.QPoint].connect(self.folder_right_menu_show)  #鼠标右键单击
        self.folderWidget.itemDoubleClicked.connect(self.on_cd_action)  #鼠标左键双击
        self.folderWidget.setMouseTracking(True)
        self.folderWidget.itemEntered.connect(lambda item: self.on_look_action(item, self.folderWidget))  #鼠标悬浮
        
        self.folderWidget_2.customContextMenuRequested[QtCore.QPoint].connect(self.folder_right_menu_show2)  #鼠标右键单击
        self.folderWidget_2.itemDoubleClicked.connect(self.on_cd_action2)  #鼠标左键双击
        self.folderWidget_2.setMouseTracking(True)
        self.folderWidget_2.itemEntered.connect(lambda item: self.on_look_action(item, self.folderWidget_2))  #鼠标悬浮

        self.connectButton.clicked.connect(self.on_conn_btn_clk)  # 点击 conncect按钮
        self.goButton.clicked.connect(self.on_cd_btn_clk)     # remote cd按钮
        self.goButton_2.clicked.connect(self.on_cd_btn2_clk)  # local cd按钮

        self.actionNew_Folder.triggered.connect(self.on_mkdir_action)
        self.actionUpload.triggered.connect(lambda: self.on_upload_action())

        self.actionPort.triggered.connect(self.on_action_port_triggered)
        self.actionPassive.triggered.connect(self.on_action_passive_triggered)

        self.actionSYSTEM.triggered.connect(self.on_action_system_triggered)
        self.actionTYPE.triggered.connect(self.on_action_type_triggered)

    def on_conn_btn_clk(self):
        '''点击连接按钮'''
        if self.has_connected == False:  #尚未建立连接
            # 则根据输入连接到服务器
            ip = self.ipInput.text()
            port = self.portInput.text()
            user = self.userInput.text()
            password = self.passwordInput.text()            
            print("Start connection to %s:%s" %(ip,port))
            try:
                self.client.cmd_conn = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                self.client.cmd_conn.connect((ip,int(port)))
                recv_line = self.client.cmd_conn.makefile().readline()
                sys.stdout.write(recv_line)
                self.cmdBrowser.append("Connect to FTP server successfully!")
            except:
                self.cmdBrowser.append("Error : Fail to connect %s:%s." %(ip,port))
                return
            if user == "" or password =="":
                self.cmdBrowser.append("Error : Need username and password to login")
                return

            self.client.command = "login %s %s"%(user,password)
            if not "230" in self.client.handle_login():                
                self.cmdBrowser.append("Error: login failed!")
            else:
                self.goButton.setEnabled(True)
                self.has_connected = True
                self.cmdBrowser.append("User %s login successfully!" %user)
                self.connectButton.setText("disconnect")
                self.menubar.setEnabled(True)
                self.statusbar.setEnabled(True)
                self.render_file_list(self.folderWidget,0)
                self.render_file_list(self.folderWidget_2,1)
        else:  #如果已经建立连接
            self.client.command = "QUIT"
            self.client.handle_COMMON()            
            self.cmdBrowser.append("Good Bye.\n")
            self.connectButton.setText("connect")
            self.folderWidget.clearContents()
            self.folderWidget.setRowCount(0)
            self.has_connected = False
            self.menubar.setEnabled(False)
            self.statusbar.setEnabled(False)
            self.goButton.setEnabled(False)

    def on_cd_btn_clk(self):
        if not self.has_connected:
            return
        dir = self.navigationBar.text()
        self.client.command = "CWD "+dir
        if "250" in self.client.handle_COMMON():
            self.remote_path = self.client.handle_pwd()
            self.navigationBar.setText(self.remote_path)
            self.cmdBrowser.append("CWD to path \"%s\""%self.remote_path)            
            self.render_file_list(self.folderWidget,0)
        else:
            self.cmdBrowser.append("Error: CWD failed!")
            self.navigationBar.setText(self.remote_path)

    def on_cd_btn2_clk(self):
        dir = self.navigationBar_2.text()
        p = Path(self.local_path)
        abs_path = str(p.joinpath(dir).resolve())
        if not os.path.exists(abs_path):
            self.cmdBrowser.append("Error: No such folder in this computer")
            return
        self.local_path = abs_path
        self.navigationBar_2.setText(self.local_path)
        self.cmdBrowser.append("Local: CD to path \"%s\""%self.local_path)            
        self.render_file_list(self.folderWidget_2,1)

    def render_file_list(self, folderWidget, mode=0):
        if(mode == 0):
            '''通过ls命令渲染文件夹预览'''
            self.client.command = "ls"
            self.client.handle_ls(self.trans_mode)
            path_list = self.client.list_result.strip().split("\n")
        else:
            (status, output) = subprocess.getstatusoutput('ls -l %s'%self.local_path)
            path_list = output.strip().split("\n")

        if(len(path_list)>1):
            total = path_list.pop(0)
            folderWidget.clearContents()
            folderWidget.setRowCount(len(path_list)+1)
            #文件名
            name_item = QTableWidgetItem("..")
            name_item.setFlags(Qt.ItemIsSelectable | Qt.ItemIsEnabled)
            folderWidget.setItem(0, 0, name_item)
            #文件类型
            file_type = "folder"
            type_item = QTableWidgetItem(file_type)
            type_item.setFlags(Qt.ItemIsEnabled)
            folderWidget.setItem(0, 1, type_item)

            index = 1
            for line in path_list:
                info = line.split(maxsplit=8)
                #文件名
                name_item = QTableWidgetItem(info[8])
                name_item.setFlags(Qt.ItemIsSelectable | Qt.ItemIsEnabled)
                folderWidget.setItem(index, 0, name_item)
                #文件类型
                file_type = get_type(file_name=info[8], access_string=info[0])
                type_item = QTableWidgetItem(file_type)
                type_item.setFlags(Qt.ItemIsEnabled)
                folderWidget.setItem(index, 1, type_item)
                #文件大小
                if file_type == "folder":
                    size = "--"
                else:
                    size = translate_size(info[4])
                size_item = QTableWidgetItem(size)
                size_item.setFlags(Qt.ItemIsEnabled)
                folderWidget.setItem(index, 2, size_item)
                #文件的最近修改时间
                time_item = QTableWidgetItem(" ".join(info[5:8]))
                time_item.setFlags(Qt.ItemIsEnabled)
                folderWidget.setItem(index, 3, time_item)
                #文件的权限
                permission_item = QTableWidgetItem(info[0])
                permission_item.setFlags(Qt.ItemIsEnabled)
                folderWidget.setItem(index, 4, permission_item)
                index += 1
        else:
            folderWidget.clearContents()
            folderWidget.setRowCount(1)
            #文件名
            name_item = QTableWidgetItem("..")
            name_item.setFlags(Qt.ItemIsSelectable | Qt.ItemIsEnabled)
            folderWidget.setItem(0, 0, name_item)
            #文件类型
            file_type = "folder"
            type_item = QTableWidgetItem(file_type)
            type_item.setFlags(Qt.ItemIsEnabled)
            folderWidget.setItem(0, 1, type_item)


    def folder_right_menu_show(self, point):
        '''当右键点击文件浏览窗口时，根据选中项弹出相关操作'''
        if not self.has_connected:
            return
        popMenu = QMenu()
        index = self.folderWidget.indexAt(point)
        row = index.row()
        column = index.column()
        if row >= 0 and column == 0: #如果右键文件名一列
            file_name = self.folderWidget.item(row, 0).text()
            file_type = self.folderWidget.item(row, 1).text()
            if file_type == "folder" and (not file_name == ".." ):  #如果该文件是文件夹
                popMenu.addAction(QAction(text="rename", parent=popMenu, triggered=lambda: self.on_rename_action(old_name=file_name)))
                popMenu.addAction(QAction(text="remove", parent=popMenu, triggered=lambda: self.on_rmdir_action(file_name)))
            elif not file_type == "folder":  #如果该文件不是文件夹
                popMenu.addAction(QAction(text="rename", parent=popMenu, triggered=lambda: self.on_rename_action(old_name=file_name)))
                popMenu.addAction(QAction(text="download", parent=popMenu, triggered=lambda: self.on_download_action(file_name)))
                popMenu.addAction(QAction(text="resume download", parent=popMenu, triggered=lambda: self.on_redownload_action(file_name)))
                popMenu.addAction(QAction(text="delete", parent=popMenu, triggered=lambda: self.on_delete_action(file_name)))
        else: #其余情况
            popMenu.addAction(QAction(text="create folder", parent=popMenu, triggered=lambda: self.on_mkdir_action()))
            popMenu.addAction(QAction(text="upload file", parent=popMenu, triggered=lambda: self.on_upload_action()))
        popMenu.exec_(QtGui.QCursor.pos())

    def folder_right_menu_show2(self, point):
        '''当右键点击文件浏览窗口时，根据选中项弹出相关操作'''
        if not self.has_connected:
            return
        popMenu = QMenu()
        index = self.folderWidget_2.indexAt(point)
        row = index.row()
        column = index.column()
        if row >= 0 and column == 0: #如果右键文件名一列
            file_name = self.folderWidget_2.item(row, 0).text()
            file_type = self.folderWidget_2.item(row, 1).text()
            if not file_type == "folder":  #如果该文件不是文件夹
                popMenu.addAction(QAction(text="upload", parent=popMenu, triggered=lambda: self.on_upload_clk(file_name=file_name)))
        popMenu.exec_(QtGui.QCursor.pos())

    def on_cd_action(self, item):
        '''双击文件夹时响应'''
        if not self.has_connected:
            return
        dir = item.text()
        type = self.folderWidget.item(item.row(), 1).text()
        if type == "folder":
            self.client.command = "CWD "+dir
            if "250" in self.client.handle_COMMON():
                self.remote_path = self.client.handle_pwd()
                self.navigationBar.setText(self.remote_path)
                self.cmdBrowser.append("CWD to path \"%s\""%self.remote_path)            
                self.render_file_list(self.folderWidget,0)
                self.render_file_list(self.folderWidget_2,1)
            else:
                self.cmdBrowser.append("Error: CWD failed!")
                self.navigationBar.setText(self.remote_path)

    def on_cd_action2(self, item):
        '''双击文件夹时响应'''
        dir = item.text()
        type = self.folderWidget_2.item(item.row(), 1).text()
        if type == "folder":            
            p = Path(self.local_path)
            absolute_path = str(p.joinpath(dir).resolve())
            if not os.path.exists(absolute_path):
                self.cmdBrowser.append("Error: No such folder in this computer")
                return
            self.local_path = absolute_path
            self.navigationBar_2.setText(self.local_path)
            self.cmdBrowser.append("Local: CD to path \"%s\""%self.local_path)            
            self.render_file_list(self.folderWidget_2,1)

    def on_download_action(self, file_name):
        '''使用download下载文件file_name'''
        local_path = QFileDialog.getExistingDirectory(self.centralwidget, 'Download', './')
        new_thread = threading.Thread(target=self.new_thread_download, args=(file_name,local_path,1,))
        new_thread.start()
    
    def new_thread_download(self, file_name, local_path, mode):
        if not self.state == NO_TASK:
            self.cmdBrowser.append("Error: Can't do that, a file transfer task is running")
            return
        if local_path:
            indexes = self.folderWidget.selectedIndexes()
            for r in indexes:
                file_name = self.folderWidget.item(r.row(), 0).text()
                file_type = self.folderWidget.item(r.row(), 1).text()
                c_path = self.remote_path
                if(c_path == "/"):
                    c_path = ""
                if(mode == 1):
                    self.cmdBrowser.append("Start to download file \"%s/%s\" to \"%s\""%(c_path,file_name,local_path))
                elif(mode == 2):
                    self.cmdBrowser.append("Start to resume download file \"%s/%s\" to \"%s\""%(c_path,file_name,local_path))
                self.state = DOWNLOADING
                self.client.command = "download "+ file_name + " " + local_path
                if "226" in self.client.handle_download(self.trans_mode):
                    self.cmdBrowser.append("File \"%s/%s\" download successfully!"%(c_path,file_name))
                else:
                    self.cmdBrowser.append("Error: File \"%s/%s\" download failed"%(c_path,file_name))
                self.state = NO_TASK
        else:
            self.cmdBrowser.append("Error: Download fail, need file path")
        self.render_file_list(self.folderWidget,0)
        self.render_file_list(self.folderWidget_2,1)

    def on_redownload_action(self, file_name):
        '''重新下载文件'''
        local_file, ftype = QFileDialog.getOpenFileName(self.centralwidget, "Resume Download", './')
        if local_file:
            local_path = os.path.split(local_file)[0]
            if not file_name == os.path.split(local_file)[1]:
                self.cmdBrowser.append("Error: Resume file name is wrong")
                return            
            rest_start = os.path.getsize(local_file)
            self.client.command = "REST %s"%rest_start
            self.client.handle_COMMON()
            new_thread = threading.Thread(target=self.new_thread_download, args=(file_name,local_path,2,))
            new_thread.start()

    def on_upload_clk(self, file_name):
        local_path = self.local_path + "/" + file_name
        new_thread = threading.Thread(target=self.new_thread_upload, args=(local_path,))
        new_thread.start()

    def on_upload_action(self):
        '''上传文件'''
        local_path, ftype = QFileDialog.getOpenFileName(self.centralwidget, "Upload File", './')
        if local_path:
            new_thread = threading.Thread(target=self.new_thread_upload, args=(local_path,))
            new_thread.start()

    def new_thread_upload(self, local_path):
        if not self.state == NO_TASK:
            self.cmdBrowser.append("Error: Can't do that, a file transfer task is running")
            return        
        file_name = os.path.split(local_path)[1]
        c_path = self.remote_path
        if(c_path == "/"):
            c_path = ""
        self.state = UPLOADING
        self.cmdBrowser.append("Start to upload file \"%s\" to \"%s/%s\""%(local_path,c_path,file_name))
        self.client.command = "upload %s"%local_path
        if "226" in self.client.handle_upload():
            self.cmdBrowser.append("Upload file \"%s\" to \"%s/%s\" successfully"%(local_path,c_path,file_name))
        else:
            self.cmdBrowser.append("Error: Upload fail")
        self.state = NO_TASK
        self.render_file_list(self.folderWidget,0)
        self.render_file_list(self.folderWidget_2,1)

    def on_mkdir_action(self):
        '''创建文件夹'''
        dir, ok = QInputDialog.getText(self.centralwidget, "Input name","New Name")
        if ok:
            self.client.command = "MKD "+dir
            if "257" in self.client.handle_COMMON():
                self.cmdBrowser.append("Create new folder successfully!")
                self.render_file_list(self.folderWidget,0)
            else:
                self.cmdBrowser.append("Error: Fail to create new folder")

    def on_rmdir_action(self, file_name):
        '''删除文件夹file_name'''
        self.client.command = "RMD "+file_name
        if "250" in self.client.handle_COMMON():
            self.cmdBrowser.append("Remove folder successfully!")
            self.render_file_list(self.folderWidget,0)
        else:
            self.cmdBrowser.append("Error: Fail to remove the folder")

    def on_delete_action(self,file_name):
        '''删除文件file_name'''
        self.client.command = "DELE "+file_name
        if "250" in self.client.handle_COMMON():
            self.cmdBrowser.append("Delete file successfully!")
            self.render_file_list(self.folderWidget,0)
        else:
            self.cmdBrowser.append("Error: Fail to delete the file")

    def on_rename_action(self,old_name):
        '''修改文件名'''
        new_name, ok = QInputDialog.getText(self.centralwidget, "Rename File", "New Name")
        if ok:
            self.client.command = "mv "+old_name + " " +new_name
            if "200" in self.client.handle_mv():
                self.cmdBrowser.append("Rename file successfully!")
                self.render_file_list(self.folderWidget,0)
            else:
                self.cmdBrowser.append("Error: Fail to rename the file")

    def on_look_action(self, item, widget):
        '''鼠标经过时响应'''
        widget.setToolTip(item.text())

    def on_action_port_triggered(self):
        '''设置port模式'''
        if not self.has_connected:
            return
        #self.trans_mode = "PORT"
        self.cmdBrowser.append("Transfer mode set to PORT")

    def on_action_passive_triggered(self):
        '''设置Passive模式'''
        if not self.has_connected:
            return
        self.trans_mode = "PASV"
        self.cmdBrowser.append("Transfer mode set to PASV")


    def on_action_system_triggered(self):
        '''查询system'''
        if not self.has_connected:
            return
        self.client.command = "SYST"
        self.cmdBrowser.append("SYST: UNIX Type: L8")

    def on_action_type_triggered(self):
        '''查询type信息'''
        if not self.has_connected:
            return
        self.client.command = "TYPE I"
        self.cmdBrowser.append("TYPE: Type set to I")

    def closeEvent(self, event):
        '''单击退出时响应'''
        if self.has_connected:
            self.client.command = "QUIT"
            self.client.handle_COMMON()


if __name__ == '__main__':
    QApplication.setAttribute(Qt.AA_EnableHighDpiScaling)
    app = QApplication(sys.argv)
    clientWindow = ClientWindow()