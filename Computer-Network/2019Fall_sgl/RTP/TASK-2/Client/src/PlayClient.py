from tkinter import *
import tkinter.font as TKFont
import tkinter.messagebox as MessageBox
from PIL import Image, ImageTk
from PIL import ImageFile
ImageFile.LOAD_TRUNCATED_IMAGES = True
import socket, threading, sys, traceback, os
import random
import time
from math import *

from RtpPacket import RtpPacket
from Constants import Constants


class PlayClient:
	'''
	用于播放视频的客户端
	'''
	
	#初始化系列函数
	def __init__(self, master, TheServerIP, TheServerPort, TheFileName, TheStartPlace, TheSession, WhetherHasSubtitle):
		'''
        描述：初始化RTP客户端
        参数：父控件，服务器的IP和端口，文件名,开始播放的位置
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
		self.Session = TheSession
		self.RequestSent = ""
		self.Valid = True
		self.Status = Constants.RTP_TRANSPORT_INIT

		#控制，数据连接顺序码和数据连接的帧
		self.ControlSequence = 0
		self.DataSequence = 0
		self.PictureFrame = TheStartPlace
		self.PicturePlay = TheStartPlace

		#视频信息--总帧数，缓冲区时间，帧率
		self.PicturePerSecond = Constants.UNDEFINED_NUMBER
		self.TotalFrameNumber = Constants.UNDEFINED_NUMBER
		self.BufferTime = 10

		#文件相关，包括要播放的视频文件名，缓存文件和接收文件的文件格式和目录名
		self.FileName = TheFileName
		self.SubtitleDir = "Info"
		self.CacheDirPicture = "CachePicture"
		self.CacheFront = "Cache_"
		self.PictureBack = ".jpg"
		self.SubtitleBack = ".srt"

		#进度条信息
		self.ScalerValueMax = Constants.UNDEFINED_NUMBER

		#播放速率
		self.CurrentPlaySpeed = 1

		#初始的播放位置
		self.StartPlace = TheStartPlace

		#当前大小信息--长，宽
		self.PictureWidth = Constants.UNDEFINED_NUMBER
		self.PictureHeight = Constants.UNDEFINED_NUMBER
		self.PictureWidthOriginal = Constants.UNDEFINED_NUMBER
		self.PictureHeightOriginal = Constants.UNDEFINED_NUMBER
		self.PictureWidthFull = Constants.WINDOW_WIDTH
		self.PictureHeightFull = Constants.WINDOW_HEIGHT
		
		#是否全屏
		self.WhetherFullScreen = False

		#字幕用数据
		self.WhetherHasSubtitle = WhetherHasSubtitle
		self.SubtitleList = []
		#每个元素：起始帧，结束帧，字幕内容

		#初始化操作：初始化控件，初始化目录，连接服务器,开始播放
		#self.CreateWidgets()
		self.InitDir()
		self.ConnectToServer()
		self.SetupMovie()
		
	def InitDir(self):
		'''
		描述：初始化缓存目录
		参数：无
		返回：无
        '''		
		if os.path.exists(self.CacheDirPicture) == False:
			os.mkdir(self.CacheDirPicture)
		if os.path.exists(self.CacheDirPicture + '/' + str(self.Session)) == False:
			os.mkdir(self.CacheDirPicture + '/' + str(self.Session))

		return

	def CreateWidgets(self):
		'''
		描述：初始化RTP客户端的控件
        参数：无
        返回：无
		'''
		self.PictureWidthFull = self.master.winfo_screenwidth()
		self.PictureHeightFull = self.master.winfo_screenheight()

		self.master.bind("<Configure>", self.ChangeScreen)
		#图片显示
		self.Movie = Label(self.master)
		#字幕显示
		self.Subtitle = Label(self.master, font = ("Times", 20), justify = "center")
		#暂停/继续按钮		
		self.Pause = Button(self.master, width = 20, padx = 3, pady = 3)
		self.Pause["text"] = "Pause"
		self.Pause["command"] = self.PauseMovie

		#播放速率选择按钮
		self.CreateChoiceButtons()
		#进度条和时间显示
		self.CreateScaler()
		#初始化控件位置
		self.SetWidgetPlace()

	def CreateChoiceButtons(self):
		'''
		描述：初始化播放速率选择控件
        参数：无
        返回：无
		'''
		self.IntVarChoiceValue = IntVar()
		#播放速率列表
		self.PlaySpeedList = [(0.5, 0), (0.75, 1), (1, 2), (1.25, 3), (1.5, 4), (2, 5)]
		#for循环创建单选框
		self.ChoiceButtonList = []
		for Speed, Num in self.PlaySpeedList:
			TheRadioButton = Radiobutton(self.master, text = "X" + str(Speed), value = Num, command = self.ChangePlaySpeed\
			, variable = self.IntVarChoiceValue)
			self.ChoiceButtonList.append(TheRadioButton)
		#设置初始值
		self.IntVarChoiceValue.set(2)

	def CreateScaler(self):
		'''
		描述：初始化播放进度条和时间显示
        参数：无
        返回：无
		'''
		#播放进度条
		self.ScalerValueMax = round(self.TotalFrameNumber / self.PicturePerSecond)
		TheLabel = self.GetPlayTime(self.StartPlace) + '/' + self.GetPlayTime(self.TotalFrameNumber)
		#进度条时间
		self.ProgressShow = Label(self.master, width = 20)
		self.ProgressShow.configure(text = TheLabel)
		#进度条本身
		self.Scaler = Scale(self.master, label = '', from_ = 0, to = self.ScalerValueMax, orient = HORIZONTAL,
             length = self.PictureWidth - 120, showvalue = 0, resolution = 1, command = self.ChangeScaler)
		#初始值设置
		CurrentProcess = round(self.StartPlace / self.TotalFrameNumber * self.ScalerValueMax) 
		self.Scaler.set(CurrentProcess)

	def SetWidgetPlace(self):
		'''
		描述：设置控件位置
        参数：无
        返回：无
		'''
		print(self.PictureWidth, self.PictureHeight)
		self.master.geometry(str(self.PictureWidth) + "x" + str(self.PictureHeight + 120))
		self.Movie.place(x = 0, y = 0, anchor = NW)
		self.Subtitle.place(x = self.PictureWidth / 2, y = self.PictureHeight * 0.9, anchor = N)
		self.Pause.place(x = self.PictureWidth / 2, y = self.PictureHeight + 80, anchor = N)
		self.ProgressShow.place(x = 10, y = self.PictureHeight + 30, anchor = NW)
		#self.FullScreen.place(x = self.PictureWidth - 200, y = self.PictureHeight + 80, anchor = N)
		self.Scaler.place(x = 10, y = self.PictureHeight + 50, anchor = NW)
		for i in range(len(self.ChoiceButtonList)):
			self.ChoiceButtonList[i].place(x = self.PictureWidth - 100, y = self.PictureHeight + 10 + 18 * i, anchor = NW)


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

	#绑定按钮和事件处理相关操作，包括SETUP，PLAY，PAUSE，RESUME，TEARDOWN, GETPARAMETER，修改进度条，修改倍速，全屏

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
		Constants.CurrentFrameBuffer = self.PicturePlay	
		self.master.destroy() 
		
		'''try:
			#os.chdir(self.CacheDirPicture)
			for i in range(self.PictureFrame):
				TheCacheName = self.GetPictureCacheFileName(i + 1)
				os.remove(TheCacheName)  
		except:
			donothing = True'''

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
	
	def GetVideoParameter(self):
		'''
		描述：GetParameter操作
        参数：无
        返回：无
		'''	
		if self.Status == Constants.RTP_TRANSPORT_READY:
			self.SendControlRequest("GET_PARAMETER")

	def SetStartPlace(self):
		'''
		描述：SetStartPlace操作
        参数：无
        返回：无
		'''	
		if self.Status == Constants.RTP_TRANSPORT_READY:
			self.SendControlRequest("SET_START_PLACE")

	def ChangeScaler(self, TheScalePlace):
		'''
		描述：滚动条变化事件
        参数：无
        返回：无
		'''	
		if self.Status != Constants.RTP_TRANSPORT_INIT:
			TheFrame = round(self.TotalFrameNumber * int(TheScalePlace) / self.ScalerValueMax)
			print("The File has loaded to the time of ", self.GetPlayTime(self.PictureFrame))
			print("The File has played to the time of ", self.GetPlayTime(self.PicturePlay))

			#进度条变化的足够大
			if abs(TheFrame - self.PicturePlay) > self.PicturePerSecond:
				self.PicturePlay = TheFrame
				self.UpdateProcess()
	
	def ChangePlaySpeed(self):
		'''
		描述：播放速率变化，处理播放速率选择事件
        参数：无
        返回：无
		'''	
		for i in range(len(self.PlaySpeedList)):
			if (self.IntVarChoiceValue.get() == i):
				self.CurrentPlaySpeed = self.PlaySpeedList[i][0]
				break 

	#控制连接相关函数，包括发送请求，处理收到的回复，处理请求等
	def SendControlRequest(self, TheRequestType):
		'''
		描述：向服务器发送控制请求
        参数：请求类型
        返回：无
		'''	
		if TheRequestType == "SETUP" and self.Status == Constants.RTP_TRANSPORT_INIT:
			self.ControlSequence += 1
			threading.Thread(target = self.ReceiveControlReply).start()

			TheRequest = 'SETUP ' + self.FileName + ' RTSP/1.0\n' \
			+ 'CSeq: ' + str(self.ControlSequence) + \
			'\nSession: ' + str(self.Session) + \
			'\nTransport: RTP/UDP; client_port= ' + str(self.DataPort)
			self.RequestSent = "SETUP"
		
		elif TheRequestType == "GET_PARAMETER":
			self.ControlSequence += 1
			TheRequest = 'GET_PARAMETER ' + self.FileName + ' RTSP/1.0\n' \
			+ 'CSeq: ' + str(self.ControlSequence) \
			+ '\nSession: ' + str(self.Session)
			self.RequestSent = "GET_PARAMETER"

		elif TheRequestType == "SET_START_PLACE" and self.Status == Constants.RTP_TRANSPORT_READY:
			self.ControlSequence += 1
			TheRequest = 'SET_START_PLACE ' + self.FileName + ' RTSP/1.0\n' \
			+ 'CSeq: ' + str(self.ControlSequence) \
			+ '\nSession: ' + str(self.Session) \
			+ '\nStartPlace: ' + str(self.StartPlace)
			self.RequestSent = "SET_START_PLACE"

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
						self.Status = Constants.RTP_TRANSPORT_READY
						self.OpenDataPort()
						self.GetVideoParameter()
					elif self.RequestSent == "GET_PARAMETER":
						self.SetVideoParameter(str(TheReply))
						self.ParseSubtitle()
						self.CreateWidgets()
						self.SetStartPlace()
					elif self.RequestSent == "SET_START_PLACE":
						self.PlayMovie()
					elif self.RequestSent == "PLAY":
						self.Status = Constants.RTP_TRANSPORT_PLAYING
					elif self.RequestSent == "PAUSE":
						self.Status = Constants.RTP_TRANSPORT_READY
						self.Pause["text"] = "Resume"
						self.Pause["command"] = self.ResumeMovie
						#self.PlayEvent.set()
					elif self.RequestSent == "RESUME":
						self.Status = Constants.RTP_TRANSPORT_PLAYING
						self.Pause["text"] = "Pause"
						self.Pause["command"] = self.PauseMovie
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
		WhetherStartedPlay = False
		while True:
			try:
				TheData, TheAddress = self.DataSocket.recvfrom(Constants.DATA_PACKET_SIZE)
				#控制接收文件
				if TheData:
					ThePacket = RtpPacket()
					ThePacket.decode(TheData)
					
					CurrentSequenceNum = ThePacket.seqNum()
					CurrentMarker = ThePacket.Marker()

					#丢弃其余数据包
					if self.DataSequence == CurrentSequenceNum - 1:
						#print("received packet ", CurrentSequenceNum)
						#回复ACK
						ACKMessage = "ACK " + str(CurrentSequenceNum)
						self.DataSocket.sendto(ACKMessage.encode(), TheAddress)

						#判断是否新图片
						if CurrentMarker == 0:
							self.PictureFrame = self.PictureFrame + 1
							#print(self.PictureFrame)

						#写入
						self.DataSequence = CurrentSequenceNum
						self.WritePictureFrame(ThePacket.getPayload())

				#控制播放图片
				if self.PictureFrame - self.PicturePlay >= (self.PicturePerSecond * self.BufferTime):
					if WhetherStartedPlay == False:
						WhetherStartedPlay = True
						threading.Thread(target = self.UpdateMovie).start()

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
	
	def UpdateMovie(self):
		'''
		描述：控制视频播放
        参数：文件名
        返回：无
		'''	
		while True:
			if self.Valid == False:
				break
			if self.Status != Constants.RTP_TRANSPORT_PLAYING:
				time.sleep(1 / self.PicturePerSecond)
				continue
			if self.PicturePlay >= self.PictureFrame:
				time.sleep(1 / self.PicturePerSecond)
				continue
			#print("The player has received ", self.PictureFrame, " frames")
			#print("The player has played ", self.PicturePlay, " frames")
			self.PicturePlay = self.PicturePlay + 1
			TheFileName = self.GetPictureCacheFileName(self.PicturePlay)
			try:
				self.UpdatePictureShow(TheFileName)
			except:
				donothing = True

			#更新进度条和时间显示
			self.UpdateScalerAndProcessWhenPlay()	

			#平衡opencv运算带来的延迟问题
			if self.WhetherFullScreen == True:
				time.sleep(1 / self.PicturePerSecond / self.CurrentPlaySpeed/ 1.7)
			else:
				time.sleep(1 / self.PicturePerSecond / self.CurrentPlaySpeed)


	def UpdatePictureShow(self, TheImageFileName):
		'''
		描述：更新图片显示
        参数：文件名
        返回：无
		'''	
		ThePhoto = Image.open(TheImageFileName)
		if self.WhetherFullScreen == True:
			ThePhoto = ThePhoto.resize((round(self.PictureWidthFull * 0.97), round(self.PictureHeightFull * 0.97)))
		ThePhotoShow = ImageTk.PhotoImage(ThePhoto)
		self.Movie.configure(image = ThePhotoShow) 
		self.Movie.image = ThePhotoShow

		#字幕显示
		if self.WhetherHasSubtitle:
			self.Subtitle["text"] = self.UpdateSubtitle()
		#self.Subtitle.configure(self.UpdateSubtitle())

	def UpdateSubtitle(self):
		'''
		描述：更新字幕显示
        参数：无
        返回：字幕
		'''	
		TheShow = ""
		for item in self.SubtitleList:
			if self.PicturePlay >= item["Start"] and self.PicturePlay <= item["End"]:
				TheShow = item["Content"]
				break
		return TheShow
		

	def UpdateScalerAndProcessWhenPlay(self):
		'''
		描述：在播放中动态更新进度条和播放时间显示
        参数：无
        返回：无
		'''	
		CurrentProcess = round(self.PicturePlay / self.TotalFrameNumber * self.ScalerValueMax) 
		PreviousProcess = int(self.Scaler.get())
		if PreviousProcess != CurrentProcess:
			self.Scaler.set(CurrentProcess)
			CurrentLabelShow = self.GetPlayTime(self.PicturePlay) + '/' + self.GetPlayTime(self.TotalFrameNumber)
			self.ProgressShow.configure(text = CurrentLabelShow)

	def UpdateProcess(self):
		'''
		描述：更新播放时间显示
        参数：无
        返回：无
		'''	
		CurrentLabelShow = self.GetPlayTime(self.PicturePlay) + '/' + self.GetPlayTime(self.TotalFrameNumber)
		self.ProgressShow.configure(text = CurrentLabelShow)

	#设置全屏和退出全屏
	def ChangeScreen(self, event):
		'''
		描述：处理进入和退出全屏事件
        参数：无
        返回：无
		'''	
		if self.WhetherFullScreen != True and \
		event.width >= (self.PictureWidthFull * 0.95) and event.height >= (self.PictureHeightFull * 0.95):
			#进入全屏
			print("in")
			self.WhetherFullScreen = True
			self.PictureWidth = round(self.PictureWidthFull * 0.97)
			self.PictureHeight = round(self.PictureHeightFull * 0.97)
			self.SetWidgetPlace()
			time.sleep(0.01)
			return
		if self.WhetherFullScreen == True and \
		(event.width < (self.PictureWidthFull * 0.95) or event.height < (self.PictureHeightFull * 0.95)):
			#退出全屏
			print("out")
			self.WhetherFullScreen = False
			self.PictureWidth = self.PictureWidthOriginal
			self.PictureHeight = self.PictureHeightOriginal
			self.SetWidgetPlace()
			time.sleep(0.01)
			return

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
		TheFileName = self.CacheDirPicture + '/' + str(self.Session) + '/' + self.CacheFront + self.FileName\
		 + '_' + str(TheSequenceNum) + self.PictureBack
		return TheFileName
	
	def GetSubtitleFileName(self):
		'''
		描述：根据视频文件名获取字幕文件名
		参数：无
		返回：文件名
        '''
		return self.SubtitleDir + '/' + str(self.Session) + '/' + self.FileName[:-4] + self.SubtitleBack

	def SetVideoParameter(self, TheReply):
		'''
		描述：获取视频的总长度，帧率信息，用于设置自身属性
		参数：返回的报文
		返回：无
        '''
		#print(TheReply)
		Lines = str(TheReply).split('\n')
		#print(Lines[-1].split())
		TheFrameNumber = int(Lines[3].split()[1])
		TheFrameRate = int(Lines[3].split()[3])
		TheFrameWidth = int(Lines[3].split()[5])
		TheFrameHeight = int(Lines[3].split()[7])
		self.TotalFrameNumber = TheFrameNumber
		self.PicturePerSecond = TheFrameRate
		self.PictureWidthOriginal = TheFrameWidth
		self.PictureHeightOriginal = TheFrameHeight
		self.PictureWidth = TheFrameWidth
		self.PictureHeight = TheFrameHeight

	def GetPlayTime(self, TheFrameNumber):
		'''
		描述：根据播放帧数计算播放时间
		参数：帧数
		返回：字符串，代表时间
        '''
		TotalSecond = round(TheFrameNumber / self.PicturePerSecond)
		TheHour = floor(TotalSecond / 3600)
		TheMinute = floor((TotalSecond - TheHour * 3600) / 60)
		TheSecond = TotalSecond % 60
		TheString = str(TheHour) + ":" + str(TheMinute).zfill(2) + ":" + str(TheSecond).zfill(2)
		return TheString

	#解析字幕对应函数
	def GetSubtitleFrame(self, TheTime):
		'''
		描述：根据字幕时间读取字幕帧数
		参数：时间字符串
		返回：对应帧数
        '''
		#print(TheTime)
		TheHour = int(TheTime[0:2])
		TheMinute = int(TheTime[3:5])
		TheSecond = int(TheTime[6:8])
		TheMili = int(TheTime[9:12])
		TheSecondNum = TheSecond + 60 * TheMinute + 3600 * TheHour + 0.001 * TheMili
		#print(self.PicturePerSecond, TheSecondNum)
		TheFrame = round(self.PicturePerSecond * TheSecondNum)
		#print(TheFrame)
		return TheFrame

	def ParseSubtitleTime(self, TheTime):
		'''
		描述：解析字幕时间
		参数：时间字符串
		返回：对应开始和结束帧数
        '''
		#print(TheTime)
		TheStartTimeStr = TheTime.split()[0]
		TheEndTimeStr = TheTime.split()[2]
		TheStartFrame = self.GetSubtitleFrame(TheStartTimeStr)
		TheEndFrame = self.GetSubtitleFrame(TheEndTimeStr)
		return TheStartFrame, TheEndFrame

	def JudgeEmpty(self, TheStr):
		'''
		描述：判断字符串是否全空
		参数：字符串
		返回：是true不是false
        '''
		WhetherEmpty = True
		for item in TheStr:
			if item != '\r' and item != '\n' and item != ' ':
				WhetherEmpty = False
				break
		return WhetherEmpty

	def ParseSubtitle(self):
		'''
		描述：解析字幕文件
		参数：无
		返回：无
        '''
		TheSubtitleName = self.GetSubtitleFileName()
		if self.WhetherHasSubtitle == True:
			try:
				File = open(TheSubtitleName, 'r')
			except:
				self.WhetherHasSubtitle = False
				return
			ContentList = File.read().split('\n')
			TheTimePlace = Constants.UNDEFINED_NUMBER
			TheContentStart = Constants.UNDEFINED_NUMBER
			TheContentEnd = Constants.UNDEFINED_NUMBER
			for i in range(len(ContentList)):
				if len(ContentList[i]) != 0:
					if ContentList[i][-1] == '\r':
						ContentList[i] = ContentList[i][:-1]
				try:
					TheNum = int(ContentList[i])
					TheTimePlace = i + 1
					TheContentStart = i + 2
				except:
					donothing = True
				if self.JudgeEmpty(ContentList[i]) == True and\
				TheTimePlace != Constants.UNDEFINED_NUMBER and TheContentStart != Constants.UNDEFINED_NUMBER:
					TheContentEnd = i - 1
					TheStartFrame, TheEndFrame = self.ParseSubtitleTime(ContentList[TheTimePlace])
					j = TheContentStart
					TheContent = ""
					while j <= TheContentEnd:
						TheContent = TheContent + ContentList[j]
						if j != TheContentEnd:
							TheContent = TheContent + '\n'
						j = j + 1
					TheInfo = {}
					TheInfo["Start"] = TheStartFrame
					TheInfo["End"] = TheEndFrame
					TheInfo["Content"] = TheContent
					self.SubtitleList.append(TheInfo)
					TheTimePlace = Constants.UNDEFINED_NUMBER
					TheContentStart = Constants.UNDEFINED_NUMBER
					TheContentEnd = Constants.UNDEFINED_NUMBER
				elif i == len(ContentList) - 1 and self.JudgeEmpty(ContentList[i]) != True and\
				TheTimePlace != Constants.UNDEFINED_NUMBER and TheContentStart != Constants.UNDEFINED_NUMBER:
					TheContentEnd = i
					TheStartFrame, TheEndFrame = self.ParseSubtitleTime(ContentList[TheTimePlace])
					j = TheContentStart
					TheContent = ""
					while j <= TheContentEnd:
						TheContent = TheContent + ContentList[j]
						if j != TheContentEnd:
							TheContent = TheContent = '\n'
					TheInfo = {}
					TheInfo["Start"] = TheStartFrame
					TheInfo["End"] = TheEndFrame
					TheInfo["Content"] = TheContent
					self.SubtitleList.append(TheInfo)
					TheTimePlace = Constants.UNDEFINED_NUMBER
					TheContentStart = Constants.UNDEFINED_NUMBER
					TheContentEnd = Constants.UNDEFINED_NUMBER
