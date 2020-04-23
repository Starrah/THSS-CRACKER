import socket, threading, sys, traceback, os
import time
from RtpPacket import RtpPacket
from io import BytesIO
from PyQt5 import QtCore, QtGui
from PyQt5.QtCore import Qt
from PyQt5.QtCore import QStringListModel
from PyQt5.QtGui import *
from PyQt5.QtWidgets import *
from Ui_mainwindow import Ui_MainWindow

class ClientWindow(QMainWindow, Ui_MainWindow):
	'''客户端窗口类'''
	INIT = 0
	READY = 1
	PLAYING = 2
	state = INIT
	
	SETUP = 0
	PLAY = 1
	PAUSE = 2
	TEARDOWN = 3
	DETAIL = 4
	SETPOS = 5

	def __init__(self, serveraddr, serverport, rtpport):
		super(ClientWindow, self).__init__()
		MainWindow = QMainWindow()
		self.setupUi(MainWindow)
		self.create_signals_ans_slots()
		self.serverAddr = serveraddr
		self.serverPort = int(serverport)
		self.rtpPort = int(rtpport)
		self.rtspSeq = 0
		self.sessionId = 0
		self.requestSent = -1
		self.connectToServer()
		self.bufferFrameNbr = 0
		self.playFrameNbr = 0
		self.speedTime = 1.0
		self.playEvent = threading.Event()
		self.pressingSlider = False
		self.btnx10.setEnabled(False)
		self.btnx15.setEnabled(True)
		self.btnx05.setEnabled(True)
		self.label.setScaledContents(True)
		self.bcgPixmap = QPixmap('bcg.jpg')
		self.label.setPixmap(self.bcgPixmap)
		self.grabKeyboard()
		MainWindow.show()
		sys.exit(app.exec_())

	def loadLyrics(self):
		lyricsName = self.fileName.split('.')[0].split('/')[1]+'.srt'
		print(lyricsName)
		try:
			f=open(lyricsName)
			index = 0
			self.lyrFrame = []
			self.lyrWord = []
			for line in f:
				if index % 4 == 1:
					self.lyrFrame.append(line.strip().split(' --> '))
				elif index % 4 == 2:
					self.lyrWord.append(line.strip())
				index += 1
				
			for i in range(len(self.lyrFrame)):
				for j in range(len(self.lyrFrame[i])):
					item = self.lyrFrame[i][j]
					a = item.split(',')
					ms = int(a[1])
					b = a[0].split(':')
					hour = int(b[0])
					minute = int(b[1])
					second = int(b[2])
					result = (3600*hour + 60*minute + second + ms/1000)*self.fps
					self.lyrFrame[i][j] = int(result)
			print(self.lyrFrame)
			self.hasLyrics = True
		except Exception:
			print("no lyrics")
			self.hasLyrics = False

	def create_signals_ans_slots(self):
		'''建立窗体中的信号与槽'''
		self.setupBtn.clicked.connect(self.setupMovie)
		self.playBtn.clicked.connect(self.playMovie)
		self.pauseBtn.clicked.connect(self.pauseMovie)
		self.tearBtn.clicked.connect(self.tearDown)
		self.btnx10.clicked.connect(self.normPlay)
		self.btnx05.clicked.connect(self.slowPlay)
		self.btnx15.clicked.connect(self.fastPlay)
		self.Slider.sliderReleased.connect(self.onSliderReleased)
		self.Slider.sliderPressed.connect(self.onSliderPressed)
		self.videoList.clicked.connect(self.chooseVideo)

	def keyPressEvent(self,event):
		if event.key() == Qt.Key_Escape and self.label.isFullScreen():
			self.label.setWindowFlags(Qt.SubWindow)
			self.label.showNormal()
			self.label.setGeometry(QtCore.QRect(10, 10, 800, 450))
		elif event.key() == Qt.Key_Space:
			if self.state == self.PLAYING:
				self.pauseMovie()
			elif self.state == self.READY:
				self.playMovie()
		elif event.key() == Qt.Key_Left:
			if self.playFrameNbr > 20:
				self.playFrameNbr -= 20
			else:
				self.playFrameNbr = 1
			print("frame change", self.playFrameNbr)
			if not (self.state == self.INIT):
				self.playEvent.set()
				time.sleep(0.1)
				self.sendRtspRequest(self.SETPOS)
		elif event.key() == Qt.Key_Right:
			if self.playFrameNbr < self.frameNum-20:
				self.playFrameNbr += 20
			else:
				self.playFrameNbr = self.frameNum-1
			print("frame change", self.playFrameNbr)
			if not (self.state == self.INIT):
				self.playEvent.set()
				time.sleep(0.1)
				self.sendRtspRequest(self.SETPOS)

	def chooseVideo(self,qModelIndex):
		self.fileName = 'video/' + self.qList[qModelIndex.row()]
		print('video name: ' + self.fileName)

	def setFullScreen(self):
		pauseFlag = False
		if self.state == self.PLAYING:
			pauseFlag = True
			self.pauseMovie()
		time.sleep(0.2)
		self.label.setWindowFlags(Qt.Dialog)
		self.label.showFullScreen()

	def normPlay(self):
		self.speedTime = 1.0 
		self.btnx10.setEnabled(False)
		self.btnx15.setEnabled(True)
		self.btnx05.setEnabled(True)

	def slowPlay(self):
		self.speedTime = 0.5
		self.btnx10.setEnabled(True)
		self.btnx15.setEnabled(True)
		self.btnx05.setEnabled(False)

	def fastPlay(self):
		self.speedTime = 1.5
		self.btnx10.setEnabled(True)
		self.btnx15.setEnabled(False)
		self.btnx05.setEnabled(True)

	def onSliderReleased(self):
		self.pressingSlider = False
		self.playFrameNbr = self.Slider.value()
		print("frame change", self.playFrameNbr)
		if not (self.state == self.INIT):
			self.playEvent.set()
			time.sleep(0.1)
			self.sendRtspRequest(self.SETPOS)
	
	def onSliderPressed(self):
		self.pressingSlider = True

	def setupMovie(self):
		"""Setup button handler."""
		if self.state == self.INIT:
			self.sendRtspRequest(self.SETUP)

	def tearDown(self):
		"""Teardown button handler."""
		self.sendRtspRequest(self.TEARDOWN)	
	
	def pauseMovie(self):
		"""Pause button handler."""
		if self.state == self.PLAYING:
			self.sendRtspRequest(self.PAUSE)
	
	def playMovie(self):
		"""Play button handler."""
		if self.state == self.READY:
			# Create a new thread to listen for RTP packets
			self.playEvent = threading.Event()
			self.playEvent.clear()
			threading.Thread(target=self.listenRtp).start()
			threading.Thread(target=self.renderFrame).start()
			self.sendRtspRequest(self.PLAY)
	
	def renderFrame(self):
		"""Render frame."""
		time.sleep(0.2)
		while True:
			if self.playEvent.isSet() or (not self.state == self.PLAYING):
				print("event set")
				return
			if self.playFrameNbr<self.frameNum and self.playFrameNbr<=self.bufferFrameNbr :
				try:
					time.sleep(1/(self.fps * self.speedTime))
					print("index: ",self.playFrameNbr, "buffer: ", self.bufferFrameNbr, "queue len: ",len(self.frameQueue))
					if self.hasLyrics:
						for i in range(len(self.lyrFrame)):
							if self.playFrameNbr>= self.lyrFrame[i][0] and self.playFrameNbr<= self.lyrFrame[i][1]:
								self.lyrLabel.setText(self.lyrWord[i])
								break
					if not self.pressingSlider:
						self.Slider.setValue(self.playFrameNbr)
					img = self.frameQueue[self.playFrameNbr]
					self.frameQueue[self.playFrameNbr] = None
					if img:
						image = QImage.fromData(img)
						pixmap = QPixmap.fromImage(image)
						self.label.setPixmap(pixmap)
					self.playFrameNbr += 1
				except Exception as e:
					print("render error: ",e)

	def listenRtp(self):		
		"""Listen for RTP packets."""
		while True:
			if self.playEvent.isSet(): 
				break
			try:
				data = self.rtpSocket.recv(62040)
				if data:
					rtpPacket = RtpPacket()
					rtpPacket.decode(data)
					
					currFrameNbr = rtpPacket.seqNum()

					self.bufferFrameNbr = currFrameNbr
					self.frameQueue[self.bufferFrameNbr] = rtpPacket.getPayload()
			except Exception as e:
				print("listen error:",e)
		
	def connectToServer(self):
		"""Connect to the Server. Start a new RTSP/TCP session."""
		self.rtspSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		try:
			self.rtspSocket.connect((self.serverAddr, self.serverPort))
		except:
			QMessageBox.warning(self,'Connection1 Failed','Connection to \'%s\' failed.' %self.serverAddr,QMessageBox.Yes)
		self.fileSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		try:
			self.fileSocket.connect((self.serverAddr, 12345))
		except:
			QMessageBox.warning(self,'Connection2 Failed','Connection to \'%s\' failed.' %self.serverAddr,QMessageBox.Yes)
		
		threading.Thread(target=self.recvFileReply).start()
		self.fileSocket.send("READFILE".encode())


	def recvFileReply(self):
		while True:
			reply = self.fileSocket.recv(1024).decode("utf-8")
			if reply: 
				print(reply)
				slm=QStringListModel()
				self.qList=reply.split(' ')
				slm.setStringList(self.qList)
				self.videoList.setModel(slm)
				if len(self.qList)>0:
					self.fileName = 'video/' + self.qList[0]
					print(self.fileName)


	def sendRtspRequest(self, requestCode):
		"""Send RTSP request to the server."""
		
		# Setup request
		if requestCode == self.SETUP and (self.state == self.INIT):
			threading.Thread(target=self.recvRtspReply).start()
			# Update RTSP sequence number.
			self.rtspSeq += 1
			
			# Write the RTSP request to be sent.
			request = 'SETUP ' + self.fileName + ' RTSP/1.0\nCSeq: ' + str(self.rtspSeq) + '\nTransport: RTP/UDP; client_port= ' + str(self.rtpPort)
			
			# Keep track of the sent request.
			self.requestSent = self.SETUP
				
		# Detail request
		elif requestCode == self.DETAIL:
			self.rtspSeq += 1
			request = 'DETAIL ' + self.fileName + ' RTSP/1.0\nCSeq: ' + str(self.rtspSeq) + '\nSession: ' + str(self.sessionId)
			self.requestSent = self.DETAIL 

		# Setpos request
		elif requestCode == self.SETPOS:
			self.rtspSeq += 1
			request = 'SETPOS ' + self.fileName + ' RTSP/1.0\nCSeq: ' + str(self.rtspSeq) + '\nSession: ' + str(self.sessionId) + '\nPos: ' + str(self.playFrameNbr)
			self.requestSent = self.SETPOS

		# Play request
		elif requestCode == self.PLAY and self.state == self.READY:
			self.rtspSeq += 1
			request = 'PLAY ' + self.fileName + ' RTSP/1.0\nCSeq: ' + str(self.rtspSeq) + '\nSession: ' + str(self.sessionId)
			self.requestSent = self.PLAY
		
		# Pause request
		elif requestCode == self.PAUSE and self.state == self.PLAYING:
			self.rtspSeq += 1
			request = 'PAUSE ' + self.fileName + ' RTSP/1.0\nCSeq: ' + str(self.rtspSeq) + '\nSession: ' + str(self.sessionId)
			self.requestSent = self.PAUSE
			
		# Teardown request
		elif requestCode == self.TEARDOWN and not self.state == self.INIT:
			self.rtspSeq += 1
			request = 'TEARDOWN ' + self.fileName + ' RTSP/1.0\nCSeq: ' + str(self.rtspSeq) + '\nSession: ' + str(self.sessionId) 
			self.requestSent = self.TEARDOWN
		else:
			return
		
		# Send the RTSP request using rtspSocket.
		self.rtspSocket.send(request.encode())
		
		print('\nData sent:\n' + request)
	
	def recvRtspReply(self):
		"""Receive RTSP reply from the server."""
		while True:
			reply = self.rtspSocket.recv(1024)
			
			if reply: 
				self.parseRtspReply(reply.decode("utf-8"))
			
			# Close the RTSP socket upon requesting Teardown
			#if self.requestSent == self.TEARDOWN:
			#	self.rtspSocket.shutdown(socket.SHUT_RDWR)
			#	self.rtspSocket.close()
			#	break
	
	def parseRtspReply(self, data):
		"""Parse the RTSP reply from the server."""
		lines = str(data).split('\n')
		seqNum = int(lines[1].split(' ')[1])
		
		# Process only if the server reply's sequence number is the same as the request's
		if seqNum == self.rtspSeq:
			session = int(lines[2].split(' ')[1])
			# New RTSP session ID

			if self.state == self.INIT:
				self.sessionId = session
			
			# Process only if the session ID is the same
			if self.sessionId == session:
				if int(lines[0].split(' ')[1]) == 200: 
					if self.requestSent == self.SETUP:
						# Update RTSP state.
						self.state = self.READY
						# Open RTP port.
						self.openRtpPort()
						self.sendRtspRequest(self.DETAIL)
					elif self.requestSent == self.PLAY:
						self.state = self.PLAYING
					elif self.requestSent == self.PAUSE:
						self.state = self.READY
						# The play thread exits. A new thread is created on resume.
						self.playEvent.set()  #设置停止标志
					elif self.requestSent == self.TEARDOWN:
						self.playEvent.set()
						self.state = self.INIT
						self.frameNum = 0
						self.bufferFrameNbr = 0
						self.playFrameNbr = 0
						self.Slider.setValue(1)
						self.label.setPixmap(self.bcgPixmap)
						self.hasLyrics = False
						self.lyrFrame = []
						self.lyrWord = []
						self.lyrLabel.setText('暂无字幕...')
					elif self.requestSent == self.SETPOS:
						self.playEvent = threading.Event()
						self.playEvent.clear()
						threading.Thread(target=self.listenRtp).start()
						threading.Thread(target=self.renderFrame).start()
					elif self.requestSent == self.DETAIL:
						info = lines[3].split(" ")
						self.length = int(info[0])
						self.width = int(info[1])
						if self.width/self.length < 9/16:
							self.label.setGeometry(QtCore.QRect(10, 10, 800, 800*self.width/self.length))
						else:
							self.label.setGeometry(QtCore.QRect(410-225*self.length/self.width, 10, 450*self.length/self.width, 450))
						self.fps = int(info[2])
						self.frameNum  = int(info[3])
						self.frameQueue = [None] * (self.frameNum+1)
						self.Slider.setMinimum(1)
						self.Slider.setMaximum(self.frameNum)
						self.Slider.setTickInterval(int(self.frameNum/200))
						self.loadLyrics()

	def openRtpPort(self):
		"""Open RTP socket binded to a specified port."""
		# Create a new datagram socket to receive RTP packets from the server
		self.rtpSocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

		# Set the timeout value of the socket to 0.5sec
		self.rtpSocket.settimeout(0.5)
		try:
			# Bind the socket to the address using the RTP port given by the client user
			self.rtpSocket.bind(("", self.rtpPort))
		except:
			QMessageBox.warning(self,'Unable to Bind','Unable to bind PORT=%d' %self.rtpPort,QMessageBox.Yes)


if __name__ == '__main__':
	try:
		serverAddr = sys.argv[1]
		serverPort = sys.argv[2]
		rtpPort = sys.argv[3]
	except:
		print("[Usage: ClientLauncher.py Server_name Server_port RTP_port Video_file]\n")
	QApplication.setAttribute(Qt.AA_EnableHighDpiScaling)
	app = QApplication(sys.argv)
	clientWindow = ClientWindow(serverAddr, serverPort, rtpPort)