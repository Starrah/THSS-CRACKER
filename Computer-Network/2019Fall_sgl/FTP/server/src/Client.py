import socket
import sys
import math



class GlobalClass:
    ControlConnection = socket.socket()         # 创建 socket 对象
    DataConnection = socket.socket()
    ListenConnection = socket.socket()
    StartPlace = 0
    WhetherConnected = False
    DataHost = "127.0.0.1"
    DataPort = -1

def InitSocket():
    '''
    描述：初始化socket（控制连接）
    参数：无
    读入：IP地址
    返回：成功true失败false
    '''


    global ClientIP
    global ServerIP
    global ControlPortServer
    global DataPortServer

    #global DataPortClient


    sys.stdout.write("Please input the IP of the Server\n")
    Host = sys.stdin.readline()[:-1] # 获取本地主机名
    Port = 21                # 设置端口号
    try:
        GlobalClass.ControlConnection.connect((Host, Port))
    except:
        sys.stdout.write("Cannot connect successfully\n")
        return False
    Return = GlobalClass.ControlConnection.recv(8192).decode('UTF-8','strict')
    sys.stdout.write(Return)
    # 获取本机计算机名称
    hostname = socket.gethostname()
    # 获取本机ip
    ClientIP = socket.gethostbyname(hostname)
    ServerIP = Host
    ControlPortServer = 21
    DataPortServer = -1
    return True

def InitPortConnection(Host, Port):
    
    '''
    描述：初始化数据连接(PORT)
    参数：Host,Port
    返回：成功true失败false
    '''

    GlobalClass.ListenConnection.close()
    GlobalClass.ListenConnection = None
    GlobalClass.ListenConnection = socket.socket()
    GlobalClass.DataConnection.close()
    GlobalClass.DataConnection = None
    GlobalClass.DataConnection = socket.socket() 
    try:
        GlobalClass.ListenConnection.bind((Host, Port))
        GlobalClass.ListenConnection.listen(5)
    except:
        return False
    return True

def InitPasvConnection(IPPortList):
    '''
    描述：初始化数据连接(PASV)
    参数：IP和Port构成的list
    返回：成功true失败false
    '''
    GlobalClass.ListenConnection.close()
    GlobalClass.ListenConnection = None
    GlobalClass.ListenConnection = socket.socket()
    GlobalClass.DataConnection.close()
    GlobalClass.DataConnection = None
    GlobalClass.DataConnection = socket.socket() 
    #try:
    Host = IPPortList[0] + '.' + IPPortList[1] + '.' + IPPortList[2] + '.' + IPPortList[3]
    Port = int(IPPortList[4]) * 256 + int(IPPortList[5])
    GlobalClass.DataHost = Host[:]
    GlobalClass.DataPort = Port
    #except:
    #    return False
    return True

def Login():
    '''
    描述：登录
    参数：无
    读入：IP地址
    返回：无
    '''



    while(1):
        Username = "anonymous\r\n"
        UsernameSend = "USER " + Username
        sys.stdout.write("Your username is anonymous\n")
        GlobalClass.ControlConnection.send(str.encode(UsernameSend))
        Return = GlobalClass.ControlConnection.recv(8192).decode('UTF-8','strict')
        sys.stdout.write(Return)
        try:
            StatusCode = int(Return.split(' ')[0])
            if StatusCode >= 200 and StatusCode < 400:
                break
        except:
            continue

    while(1):
        sys.stdout.write("Please input your password\n")
        Password = sys.stdin.readline()[:-1]
        Password = Password[:] + '\r\n'
        PasswordSend = "PASS " + Password
        GlobalClass.ControlConnection.send(str.encode(PasswordSend))
        Return = GlobalClass.ControlConnection.recv(8192).decode('UTF-8','strict')
        sys.stdout.write(Return)
        try:
            StatusCode = int(Return.split(' ')[0])
            if StatusCode >= 200 and StatusCode < 400:
                break
        except:
            continue


def SendFile(FileName, Start):
    #打开
    try:
        FilePoint = open(FileName, 'rb')
    except:
        #GlobalClass.ControlConnection.send("451 Reading File Error\r\n")
        return
    #找开始位置
    try:
        FilePoint.seek(Start, 0)
    except:
        FilePoint.close()
        #GlobalClass.ControlConnection.send("451 Cannot Find StartPlace\r\n")
        return
    #读取和发送文件
    while(1):
        try:
            Data = FilePoint.read(8192)
            if len(Data) == 0:
                break
        except:
            break
        try:
            a = GlobalClass.DataConnection.sendall(Data) 
            print(len(Data))
        except:
            FilePoint.close()
            #GlobalClass.ControlConnection.send("451 Cannot Find StartPlace\r\n")
            return
    #Kebab = b'' 
    #GlobalClass.DataConnection.sendall(Kebab)
    print("Sending Finish\n")
    FilePoint.close()
    return




def SendRequests():
    '''
    描述：发送其他各种指令
    参数：无
    读入：各种指令
    返回：无
    '''

    ConnectionType = "NO"

    while(1):
        sys.stdout.write("Please input the command\n")
        command = sys.stdin.readline()[:-1]
        if(command == 'PORT'):
            while(1):
                sys.stdout.write("Please input the port of the client in data service, which must be between 20000 and 65535\n")
                try:
                    DataPortClient = int(sys.stdin.readline()[:-1])
                    if DataPortClient >= 20000 and DataPortClient <= 65535:
                        break
                except:
                    continue
            IPList = ClientIP.split('.')
            Port0 = math.floor(DataPortClient / 256)
            Port1 = DataPortClient % 256
            #print(IPList)
            PortSend = 'PORT ' + IPList[0] + ',' + IPList[1] + ',' + IPList[2] + ',' + IPList[3] + ',' + str(Port0) + ',' + str(Port1) + '\r\n'
            #rint(PortSend)
            #print(ClientIP)
            if InitPortConnection(ClientIP, DataPortClient) == True:
                ConnectionType = "PORT"
                GlobalClass.WhetherConnected = False
                print("Open PORT Listening\n")
            GlobalClass.ControlConnection.send(str.encode(PortSend))
            Return = GlobalClass.ControlConnection.recv(8192).decode('UTF-8','strict')
            sys.stdout.write(Return)
            try:
                StatusCode = int(Return.split(' ')[0])
                if not(StatusCode >= 200 and StatusCode < 400):
                    ListenConnection.close()
            except:
                GlobalClass.ListenConnection.close()
        elif(command == 'PASV'):
            GlobalClass.ControlConnection.send(str.encode("PASV\r\n"))
            Return = GlobalClass.ControlConnection.recv(8192).decode('UTF-8','strict')
            sys.stdout.write(Return)
            try:
                StatusCode = int(Return.split(' ')[0])
                if StatusCode >= 200 and StatusCode < 400:
                    IPPortList = Return[27:-3].split(',')
                    if InitPasvConnection(IPPortList) == True:
                        ConnectionType = "PASV"
                        GlobalClass.WhetherConnected = False
                        #print('PASV link success\n')
            except:
                continue
        elif(command == 'RETR'):
            sys.stdout.write("Please input the name of the File you want to download\n")
            FileName = sys.stdin.readline()[ :-1]
            SendMessage = "RETR " + FileName + '\r\n'
            print(SendMessage)
            GlobalClass.ControlConnection.send(str.encode(SendMessage))
            if ConnectionType == "PORT":
                if(GlobalClass.WhetherConnected != True):
                    GlobalClass.DataConnection, Addr = GlobalClass.ListenConnection.accept()
                    print("Connect PORT Successful!")
                    GlobalClass.WhetherConnected = True
            elif ConnectionType == "PASV":
                if(GlobalClass.WhetherConnected != True):
                    GlobalClass.DataConnection.connect((GlobalClass.DataHost, GlobalClass.DataPort))
                    print("Connect PASV Successful!")
                    GlobalClass.WhetherConnected = True
            Return = GlobalClass.ControlConnection.recv(8192).decode('UTF-8','strict')
            sys.stdout.write(Return)
            try:
                ReturnCode = int(Return.split(' ')[0])
                if(ReturnCode != 150):
                    continue
            except:
                iii = 0
            LocalFileName = FileName.split('/')[-1]
            try:
                try:
                    File = open(LocalFileName, 'rb+')
                except:
                    if GlobalClass.StartPlace == 0:
                        File = open(LocalFileName, 'wb+')
                File.seek(GlobalClass.StartPlace, 0)
                GlobalClass.StartPlace = 0
            except:
                iii = 0
            while(1):
                ReceiveData = GlobalClass.DataConnection.recv(8192)
                if ReceiveData == None or len(ReceiveData) == 0 or (not ReceiveData):
                    break
                try:
                    File.write(ReceiveData)
                    #print(len(ReceiveData))
                except:
                    iii = 0
            try:
                File.close()
            except:
                iii = 0
            Return = GlobalClass.ControlConnection.recv(8192).decode('UTF-8','strict')
            sys.stdout.write(Return)
            if ConnectionType == "PORT":
                GlobalClass.ListenConnection.close()
                GlobalClass.ListenConnection = None
                GlobalClass.ListenConnection = socket.socket()
            GlobalClass.DataConnection.close()
            GlobalClass.DataConnection = None
            GlobalClass.DataConnection = socket.socket()    
            ConnectionType = "NO"
            GlobalClass.WhetherConnected = False

        elif(command == 'STOR'):
            sys.stdout.write("Please input the name of the File you want to send\n")
            FileName = sys.stdin.readline()[ :-1]
            SendMessage = "STOR " + FileName + '\r\n'
            print(SendMessage)
            GlobalClass.ControlConnection.send(str.encode(SendMessage))
            if ConnectionType == "PORT":
                if(GlobalClass.WhetherConnected != True):
                    GlobalClass.DataConnection, Addr = GlobalClass.ListenConnection.accept()
                    print("Connect PORT Successful!")
                    GlobalClass.WhetherConnected = True
            elif ConnectionType == "PASV":
                if(GlobalClass.WhetherConnected != True):
                    GlobalClass.DataConnection.connect((GlobalClass.DataHost, GlobalClass.DataPort))
                    print("Connect PASV Successful!")
                    GlobalClass.WhetherConnected = True
            Return = GlobalClass.ControlConnection.recv(8192).decode('UTF-8','strict')
            sys.stdout.write(Return)
            try:
                ReturnCode = int(Return.split(' ')[0])
                if(ReturnCode != 150):
                    continue
            except:
                iii = 0
            try:
                FilePoint = open(FileName, 'rb')
                FilePoint.seek(GlobalClass.StartPlace, 0)
                GlobalClass.StartPlace = 0
                Data = FilePoint.read()
                Data += b'\0'
            except:
                Data = b'\0'
            print(len(Data))
            GlobalClass.DataConnection.sendall(Data)
            
            Return = GlobalClass.ControlConnection.recv(8192).decode('UTF-8','strict')
            sys.stdout.write(Return)
            if ConnectionType == "PORT":
                GlobalClass.ListenConnection.close()
                GlobalClass.ListenConnection = None
                GlobalClass.ListenConnection = socket.socket()
            GlobalClass.DataConnection.close()
            GlobalClass.DataConnection = None
            GlobalClass.DataConnection = socket.socket()    
            ConnectionType = "NO"
            GlobalClass.WhetherConnected = False
        elif(command == 'REST'):
            while(1):
                sys.stdout.write("Please Input The Place you want to send your file\n")
                try:
                    Place = int(sys.stdin.readline()[ :-1])
                    if Place >= 0:
                        break
                except:
                    continue
            SendMessage = "REST " + str(Place) + '\r\n'
            print(SendMessage)
            GlobalClass.ControlConnection.send(str.encode(SendMessage))
            Return = GlobalClass.ControlConnection.recv(8192).decode('UTF-8','strict')
            sys.stdout.write(Return)
            try:
                Code = int(Return.split(' ')[0])
                if Code >= 200 and Code < 400:
                    GlobalClass.StartPlace = Place
                    print("Set StartPlace to %d\n" %Place)
            except:
                continue
        elif(command == "SYST"):
            SendMessage = "SYST\r\n"
            GlobalClass.ControlConnection.send(str.encode(SendMessage))
            Return = GlobalClass.ControlConnection.recv(8192).decode('UTF-8','strict')
            sys.stdout.write(Return)
        elif(command == "TYPE"):
            sys.stdout.write("Please Input the type\n")
            SendMessage = "TYPE " + sys.stdin.readline()[:-1] + '\r\n'
            GlobalClass.ControlConnection.send(str.encode(SendMessage))
            Return = GlobalClass.ControlConnection.recv(8192).decode('UTF-8','strict')
            sys.stdout.write(Return)
        elif(command == "ABOR" or command == "QUIT"):
            SendMessage = command  + '\r\n'
            GlobalClass.ControlConnection.send(str.encode(SendMessage))
            Return = GlobalClass.ControlConnection.recv(8192).decode('UTF-8','strict')
            sys.stdout.write(Return)
            break
        elif(command in ["CWD", "RMD", "MKD", "RNFR", "RNTO"]):
            sys.stdout.write("Please Input The Directory or Filename you want to operate\n")
            SendMessage = command + ' ' + sys.stdin.readline()[:-1] + '\r\n'
            print(SendMessage)
            GlobalClass.ControlConnection.send(str.encode(SendMessage))
            Return = GlobalClass.ControlConnection.recv(8192).decode('UTF-8','strict')
            sys.stdout.write(Return)
        elif(command in ["CDUP", "PWD"]):
            SendMessage = command + '\r\n'
            print(SendMessage)
            GlobalClass.ControlConnection.send(str.encode(SendMessage))
            Return = GlobalClass.ControlConnection.recv(8192).decode('UTF-8','strict')
            sys.stdout.write(Return)
        elif(command == "LIST"):
            sys.stdout.write("Please input the LIST instruction\n")
            FileName = sys.stdin.readline()[ :-1]
            if(len(FileName) == 0):
                SendMessage = "LIST\r\n"
            else:
                SendMessage = "LIST " + FileName + '\r\n'
            print(SendMessage)
            GlobalClass.ControlConnection.send(str.encode(SendMessage))
            if ConnectionType == "PORT":
                if(GlobalClass.WhetherConnected != True):
                    GlobalClass.DataConnection, Addr = GlobalClass.ListenConnection.accept()
                    print("Connect PORT Successful!")
                    GlobalClass.WhetherConnected = True
            elif ConnectionType == "PASV":
                if(GlobalClass.WhetherConnected != True):
                    print(GlobalClass.DataHost, GlobalClass.DataPort)
                    GlobalClass.DataConnection.connect((GlobalClass.DataHost, GlobalClass.DataPort))
                    print("Connect PASV Successful!")
                    GlobalClass.WhetherConnected = True
            Return = GlobalClass.ControlConnection.recv(8192).decode('UTF-8','strict')
            sys.stdout.write(Return)
            try:
                ReturnCode = int(Return.split(' ')[0])
                if(ReturnCode != 150):
                    continue
            except:
                iii = 0
            ReceiveData = GlobalClass.DataConnection.recv(8192)
            sys.stdout.write(bytes.decode(ReceiveData))
            Return = GlobalClass.ControlConnection.recv(8192).decode('UTF-8','strict')
            sys.stdout.write(Return)


if __name__ == "__main__":
    if InitSocket() == True:
        Login()
        SendRequests()

