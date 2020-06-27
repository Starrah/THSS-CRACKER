import socket
import threading

from Constants import Constants
from RTPServerManager import RTPServerManager


class RTPServer:
    '''
    RTP主服务器类
    '''
    def __init__(self):
        self.ServerIP = ""
        self.ServerControlPort = Constants.RTP_SERVER_CONTROL_PORT
        self.InitSocket()
        self.ListenLinks()


    def InitSocket(self):
        '''
        描述：初始化RTP数据连接和RTSP控制连接
        参数：无
        返回：无
        '''
        #self.DataSocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        #self.DataSocket.bind((Constants.SERVER_ADDR, Constants.DATA_PORT))
        #self.DataSequence = 0

        self.ControlSocket = socket.socket()
        self.ControlSocket.bind((self.ServerIP, self.ServerControlPort))
        self.ControlSocket.listen(5)
        self.ControlSequence = 0

    def ListenLinks(self):
        '''
        描述：监听RTSP端口，获取客户端连接
        参数：无
        返回：无
        '''
        while True:
            NewControlSocket, NewAddress = self.ControlSocket.accept()
            NewThread = threading.Thread(target = self.ManageClients, args = (NewControlSocket, NewAddress))  #t为新创建的线程
            NewThread.start()
        self.ControlSocket.close()

    def ManageClients(self, NewSocket, NewAddress):
        '''
        描述：开启新的进程，处理新的客户端请求
        参数：无
        返回：无
        '''
        print("New Client has linked in, the client is ", NewAddress)
        TheServerManager = RTPServerManager(NewSocket, NewAddress)
        print("A Client has disconnected, the client is ", NewAddress)
        return

if __name__ == "__main__":
	TheServer = RTPServer()

