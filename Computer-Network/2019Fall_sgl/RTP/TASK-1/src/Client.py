from tkinter import *
import tkinter.messagebox as MessageBox
from PIL import Image, ImageTk
import socket, threading, sys, traceback, os
import random


from RtpPacket import RtpPacket
from Constants import Constants


class Client:
	
	#初始化系列函数
	def __init__(self, master, TheServerIP, TheServerPort, TheFileName):
		'''
        描述：初始化RTP客户端
        参数：父控件，服务器的IP和端口，文件名
        返回：无
		'''
		#图形界面相关
		self.master = master
		self.master.protocol("WM_DELETE_WINDOW", self.QuitByHandler)

		#网络相关，包括服务器IP，端口信息，和自己的数据连接端口信息，还有数据，控制连接本身
		self.ClientIP = ""
		self.ServerIP = TheServerIP
		self.ServerPort = int(TheServerPort)
		self.DataPort = self.GenerateRandomPort()
		self.ControlSocket = None
		self.DataSocket = None

		#控制相关，包括Session，状态等
		self.Session = Constants.UNDEFINED_NUMBER
		self.RequestSent = ""
		self.Valid = True
		self.Status = Constants.RTP_TRANSPORT_INIT

		#控制，数据连接顺序码和数据连接的帧
		self.ControlSequence = 0
		self.DataSequence = 0
		self.PictureFrame = 0
		self.AudioFrame = 0

		#文件相关，包括要播放的视频文件名，缓存文件和接收文件的文件格式和目录名
		self.FileName = TheFileName
		self.CacheDirPicture = "CachePicture"
		self.CacheDirAudio = "CacheAudio"
		self.CacheFront = "Cache_"
		self.PictureBack = ".jpg"
		self.AudioBack = ".wav"

		#初始化操作：初始化控件，初始化目录，连接服务器
		self.CreateWidgets()
		self.InitDir()
		self.ConnectToServer()
		
	def InitDir(self):
		'''
		描述：初始化缓存目录
		参数：无
		返回：无
        '''		
		if os.path.exists(self.CacheDirPicture) == False:
			os.mkdir(self.CacheDirPicture)
		if os.path.exists(self.CacheDirAudio) == False:
			os.mkdir(self.CacheDirAudio)
		return

	def CreateWidgets(self):
		'''
		描述：初始化RTP客户端
        参数：无
        返回：无
		'''
		# Create Setup button
		self.Setup = Button(self.master, width = 20, padx = 3, pady = 3)
		self.Setup["text"] = "Setup"
		self.Setup["command"] = self.SetupMovie
		self.Setup.grid(row=1, column=0, padx=2, pady=2)
		
		# Create Play button		
		self.Start = Button(self.master, width = 20, padx = 3, pady = 3)
		self.Start["text"] = "Play"
		self.Start["command"] = self.PlayMovie
		self.Start.grid(row = 1, column = 1, padx = 2, pady = 2)
		
		# Create Pause button			
		self.Pause = Button(self.master, width = 20, padx = 3, pady = 3)
		self.Pause["text"] = "Pause"
		self.Pause["command"] = self.PauseMovie
		self.Pause.grid(row = 1, column = 2, padx = 2, pady = 2)
		
		# Create Teardown button
		self.Teardown = Button(self.master, width = 20, padx = 3, pady = 3)
		self.Teardown["text"] = "Teardown"
		self.Teardown["command"] =  self.ExitClient
		self.Teardown.grid(row = 1, column = 3, padx = 2, pady = 2)
		
		# Create a label to display the movie
		self.Label = Label(self.master, height=19)
		self.Label.grid(row = 0, column = 0, columnspan = 4, sticky = W + E + N + S, padx = 5, pady = 5) 
	
	#网络连接相关操作，包括数据端口连接服务器，控制端口开启等
	def ConnectToServer(self):
		'''
		描述：让RTSP控制连接服务器
		参数：无
		返回：无
        '''	
		self.ControlSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		try:
			self.ControlSocket.connect((self.ServerIP, self.ServerPort))
		except:
			MessageBox.showwarning('Connection Failed', 'Connection to the server at \'%s\' failed.' %self.ServerIP)
	
	def OpenDataPort(self):
		'''
		描述：开启RTP数据端口，接收服务器数据
		参数：无
		返回：无
        '''	
		self.DataSocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
		self.DataSocket.settimeout(0.5)	
		try:
			self.DataSocket.bind(("", self.DataPort))
		except:
			MessageBox.showwarning('Unable to Bind', 'Unable to bind the data link at PORT = %d' %self.DataPort)

	#绑定按钮相关操作，包括SETUP，PLAY，PAUSE，RESUME，TEARDOWN
	def SetupMovie(self):
		'''
		描述：Setup操作
        参数：无
        返回：无
		'''		
		if self.Status == Constants.RTP_TRANSPORT_INIT:
			self.SendControlRequest("SETUP")
	
	def ExitClient(self):
		'''
		描述：Teardown操作，退出和删除缓存文件
        参数：无
        返回：无
		'''	
		self.SendControlRequest("TEARDOWN")		
		self.master.destroy() 
		try:
			#os.chdir(self.CacheDirPicture)
			for i in range(self.PictureFrame):
				TheCacheName = self.GetPictureCacheFileName(i + 1)
				os.remove(TheCacheName) 
			for i in range(self.AudioFrame):
				TheCacheName = self.GetAudioCacheFileName(i + 1)
				os.remove(TheCacheName) 
		except:
			donothing = True

	def QuitByHandler(self):
		'''
		描述：Teardown操作，不过用于绑定其他方式退出，而非按钮退出
        参数：无
        返回：无
		'''	
		self.PauseMovie()
		if MessageBox.askokcancel("Quit?", "Are you sure you want to quit?"):
			self.ExitClient()
		else: # When the user presses cancel, resume playing.
			self.PlayMovie()

	def PauseMovie(self):
		'''
		描述：Pause操作
        参数：无
        返回：无
		'''	
		if self.Status == Constants.RTP_TRANSPORT_PLAYING:
			self.SendControlRequest("PAUSE")
	
	def ResumeMovie(self):
		'''
		描述：Resume操作
        参数：无
        返回：无
		'''	
		if self.Status == Constants.RTP_TRANSPORT_READY:
			self.SendControlRequest("RESUME")
	
	def PlayMovie(self):
		'''
		描述：Play操作
        参数：无
        返回：无
		'''	
		if self.Status == Constants.RTP_TRANSPORT_READY:
			# Create a new thread to listen for RTP packets
			threading.Thread(target = self.DataLinkReceive).start()
			self.PlayEvent = threading.Event()
			self.PlayEvent.clear()
			self.SendControlRequest("PLAY")
	
	#控制连接相关函数，包括发送请求，处理收到的回复，处理请求等
	def SendControlRequest(self, TheRequestType):
		'''
		描述：向服务器发送控制请求
        参数：请求类型
        返回：无
		'''	
		if TheRequestType == "SETUP" and self.Status == Constants.RTP_TRANSPORT_INIT:
			threading.Thread(target = self.ReceiveControlReply).start()
			self.ControlSequence += 1
			
			TheRequest = 'SETUP ' + self.FileName + ' RTSP/1.0\n' \
			+ 'CSeq: ' + str(self.ControlSequence) + \
			'\nTransport: RTP/UDP; client_port= ' + str(self.DataPort)
			
			self.RequestSent = "SETUP"
		
		elif TheRequestType == "PLAY" and self.Status == Constants.RTP_TRANSPORT_READY:
			self.ControlSequence += 1
			TheRequest = 'PLAY ' + self.FileName + ' RTSP/1.0\n' \
			+ 'CSeq: ' + str(self.ControlSequence) \
			+ '\nSession: ' + str(self.Session)
			self.RequestSent = "PLAY"
		
		elif TheRequestType == "PAUSE" and self.Status == Constants.RTP_TRANSPORT_PLAYING:
			self.ControlSequence += 1
			TheRequest = 'PAUSE ' + self.FileName + ' RTSP/1.0\n' \
			+ 'CSeq: ' + str(self.ControlSequence) \
			+ '\nSession: ' + str(self.Session)
			self.RequestSent = "PAUSE"

		elif TheRequestType == "RESUME" and self.Status == Constants.RTP_TRANSPORT_READY:
			self.ControlSequence += 1
			TheRequest = 'RESUME ' + self.FileName + ' RTSP/1.0\n' \
			+ 'CSeq: ' + str(self.ControlSequence) \
			+ '\nSession: ' + str(self.Session)
			self.RequestSent = "RESUME"
			
		elif TheRequestType == "TEARDOWN" and not self.Status == Constants.RTP_TRANSPORT_INIT:
			self.ControlSequence += 1
			TheRequest = 'TEARDOWN ' + self.FileName + ' RTSP/1.0\n' \
			+ 'CSeq: ' + str(self.ControlSequence) \
			+ '\nSession: ' + str(self.Session)
			self.RequestSent = "TEARDOWN"
		else:
			return
		self.ControlSocket.send(TheRequest.encode())		
		print('\nData sent:\n' + TheRequest)
	
	def ReceiveControlReply(self):
		'''
		描述：接收服务器的控制连接回复
        参数：无
        返回：无
		'''	
		while True:
			TheReply = self.ControlSocket.recv(Constants.CONTROL_SIZE)
			print(TheReply.decode("utf-8"))
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
						self.Status = Constants.RTP_TRANSPORT_READY
						self.OpenDataPort()
					elif self.RequestSent == "PLAY":
						self.Status = Constants.RTP_TRANSPORT_PLAYING
					elif self.RequestSent == "PAUSE":
						self.Status = Constants.RTP_TRANSPORT_READY
						self.PlayEvent.set()
					elif self.RequestSent == "RESUME":
						self.Status = Constants.RTP_TRANSPORT_PLAYING
					elif self.RequestSent == "TEARDOWN":
						self.Status = Constants.RTP_TRANSPORT_INIT
						self.Valid = False

	#数据连接RTP部分：接收数据，更新帧，更新显示
	def DataLinkReceive(self):		
		'''
		描述：处理服务器的控制连接回复
        参数：回复内容
        返回：无
		'''	
		while True:
			try:
				TheData = self.DataSocket.recv(Constants.DATA_PACKET_SIZE)
				if TheData:
					ThePacket = RtpPacket()
					ThePacket.decode(TheData)
					
					CurrentSequenceNum = ThePacket.seqNum()
					CurrentMarker = ThePacket.Marker()
					if CurrentMarker == 0:
						self.PictureFrame = self.PictureFrame + 1
						print("New Frame")
					print("Current Seq Num: " + str(CurrentSequenceNum))
										
					if CurrentSequenceNum > self.DataSequence: # Discard the late packet
						self.DataSequence = CurrentSequenceNum
						self.WritePictureFrame(ThePacket.getPayload())
						if CurrentMarker == 0:
							TheCacheFileName = self.GetPictureCacheFileName(self.PictureFrame - 1)
							self.UpdateMovie(TheCacheFileName)
			except:
				# Stop listening upon requesting PAUSE or TEARDOWN
				if self.PlayEvent.isSet(): 
					break
				
				#处理teardown事件
				if self.Valid == False:
					try:
						self.DataSocket.shutdown(socket.SHUT_RDWR)
						self.DataSocket.close()
					except:
						donothing = True
					break
					
	def WritePictureFrame(self, TheData):
		'''
		描述：写入图片帧
        参数：数据内容
        返回：无
		'''	
		TheCacheName = self.GetPictureCacheFileName(self.PictureFrame)
		File = open(TheCacheName, "ab")
		File.write(TheData)
		File.close()
	
	def UpdateMovie(self, ImageFile):
		'''
		描述：更新显示
        参数：文件名
        返回：无
		'''	
		ThePhoto = ImageTk.PhotoImage(Image.open(ImageFile))
		self.Label.configure(image = ThePhoto, height=288) 
		self.Label.image = ThePhoto
		
	#基本操作函数，比如随机生成端口，生成完整文件名
	def GenerateRandomPort(self):	
		'''
		描述：生成随机的自身数据端口
		参数：无
		返回：一个随机数port
        '''
		ThePort = random.randint(10001, 65535)
		return ThePort

	def GetPictureCacheFileName(self, TheSequenceNum):
		'''
		描述：根据session，序列号，前缀等生成图片缓存文件名
		参数：序列号
		返回：文件名
        '''
		TheFileName = self.CacheDirPicture + '/' + self.CacheFront + str(self.Session)\
		 + '_' + str(TheSequenceNum) + self.PictureBack
		return TheFileName

	def GetAudioCacheFileName(self, TheSequenceNum):
		'''
		描述：根据session，序列号，前缀等生成音频缓存文件名
		参数：序列号
		返回：文件名
        '''
		TheFileName = self.CacheDirAudio + '/' + self.CacheFront + str(self.Session)\
		 + '_' + str(TheSequenceNum) + self.AudioBack
		return TheFileName

if __name__ == "__main__":
	Root = Tk()
	TheClient = Client(Root, Constants.SERVER_ADDR, Constants.SERVER_CONTROL_PORT, "test/test")
	Root.mainloop()
