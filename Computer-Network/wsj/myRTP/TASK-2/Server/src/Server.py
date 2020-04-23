from random import randint
import sys, traceback, threading, socket
from VideoStream import VideoStream
from RtpPacket import RtpPacket
import os

class Server:
	SETUP = 'SETUP'
	PLAY = 'PLAY'
	PAUSE = 'PAUSE'
	TEARDOWN = 'TEARDOWN'
	DETAIL = 'DETAIL'
	SETPOS = 'SETPOS'
	
	INIT = 0
	READY = 1
	PLAYING = 2
	state = INIT

	OK_200 = 0
	FILE_NOT_FOUND_404 = 1
	CON_ERR_500 = 2
	DETAIL_200 = 3

	clientInfo = {}
	
	def __init__(self, clientInfo):
		self.framePos = 0
		self.clientInfo = clientInfo
		
	def run(self):
		threading.Thread(target=self.recvFileRequest).start()
		threading.Thread(target=self.recvRtspRequest).start()

	def recvFileRequest(self):
		"""Receive file request from the client."""
		msg = ''
		for root,dirs,files in os.walk('./video'):
			for file in files:
				suffix = os.path.splitext(file)[1]
				if suffix == '.mp4' or suffix == '.flv' or suffix == '.iso' or suffix == '.mov' or suffix == '.mkv':
					msg += file
					msg += ' '
		msg = msg.strip()
		connSocket = self.clientInfo['fileSocket'][0]
		while True:            
			data = connSocket.recv(256).decode("utf-8")
			if data == "READFILE":
				print('video list: ',msg)
				connSocket.send(msg.encode())



	def recvRtspRequest(self):
		"""Receive RTSP request from the client."""
		connSocket = self.clientInfo['rtspSocket'][0]
		while True:            
			data = connSocket.recv(256)
			if data:
				print("Data received:\n" + data.decode("utf-8"))
				self.processRtspRequest(data.decode("utf-8"))
	
	def processRtspRequest(self, data):
		"""Process RTSP request sent from the client."""
		# Get the request type
		request = data.split('\n')
		line1 = request[0].split(' ')
		requestType = line1[0]
		
		# Get the media file name
		filename = line1[1]
		
		# Get the RTSP sequence number 
		seq = request[1].split(' ')
		
		# Process SETUP request
		if requestType == self.SETUP:
			if self.state == self.INIT:
				# Update state
				print("processing SETUP\n")
				
				try:
					self.clientInfo['videoStream'] = VideoStream(filename)
					self.state = self.READY
				except IOError:
					self.replyRtsp(self.FILE_NOT_FOUND_404, seq[1])
				
				# Generate a randomized RTSP session ID
				self.clientInfo['session'] = randint(100000, 999999)
				
				# Send RTSP reply
				self.replyRtsp(self.OK_200, seq[1])
				
				# Get the RTP/UDP port from the last line
				self.clientInfo['rtpPort'] = request[2].split(' ')[3]
		
		# Process PLAY request 		
		elif requestType == self.PLAY:
			if self.state == self.READY:
				print("processing PLAY\n")
				self.state = self.PLAYING
				
				# Create a new socket for RTP/UDP
				self.clientInfo["rtpSocket"] = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
				
				self.replyRtsp(self.OK_200, seq[1])
				
				# Create a new thread and start sending RTP packets
				self.clientInfo['event'] = threading.Event()
				self.clientInfo['worker']= threading.Thread(target=self.sendRtp) 
				self.clientInfo['worker'].start()
		
		elif requestType == self.SETPOS:
			print("processing SETPOS\n")
			self.framePos = int(request[3].split(" ")[1])
			print("set framepos: ",self.framePos)
			self.clientInfo['videoStream'].setPos = True
			self.replyRtsp(self.OK_200, seq[1])

		# Process PAUSE request
		elif requestType == self.PAUSE:
			if self.state == self.PLAYING:
				print("processing PAUSE\n")
				self.state = self.READY
				
				self.clientInfo['event'].set()
			
				self.replyRtsp(self.OK_200, seq[1])
		
		# Process TEARDOWN request
		elif requestType == self.TEARDOWN:
			print("processing TEARDOWN\n")
			self.state = self.INIT
			self.clientInfo['event'].set()
			self.replyRtsp(self.OK_200, seq[1])
			
			# Close the RTP socket
			#self.clientInfo['rtpSocket'].close()
		
		# Process DETAIL request
		elif requestType == self.DETAIL:
			print("processing DETAIL\n")
			
			self.replyRtsp(self.DETAIL_200, seq[1])
			
			# Close the RTP socket

	def sendRtp(self):
		"""Send RTP packets over UDP."""
		while True:
			self.clientInfo['event'].wait(0.01) 
			# Stop sending if request is PAUSE or TEARDOWN
			if self.clientInfo['event'].isSet(): 
				break 
			data = self.clientInfo['videoStream'].nextFrame(self.framePos)
			if data: 
				frameNumber = self.clientInfo['videoStream'].frameNbr()
				print('data len:',len(data),' framNum: ',frameNumber)
				try:
					address = self.clientInfo['rtspSocket'][1][0]
					port = int(self.clientInfo['rtpPort'])
					self.clientInfo['rtpSocket'].sendto(self.makeRtp(data, frameNumber),(address,port))
				except Exception as e:
					print(e)
					print("Connection Error")

	def makeRtp(self, payload, frameNbr):
		"""RTP-packetize the video data."""
		version = 2
		padding = 0
		extension = 0
		cc = 0
		marker = 0
		pt = 26 # MJPEG type
		seqnum = frameNbr
		ssrc = 0 
		
		rtpPacket = RtpPacket()
		
		rtpPacket.encode(version, padding, extension, cc, seqnum, marker, pt, ssrc, payload)
		
		return rtpPacket.getPacket()
		
	def replyRtsp(self, code, seq):
		"""Send RTSP reply to the client."""
		if code == self.OK_200:
			reply = 'RTSP/1.0 200 OK\nCSeq: ' + seq + '\nSession: ' + str(self.clientInfo['session'])
			print("reply: ",reply)
			connSocket = self.clientInfo['rtspSocket'][0]
			connSocket.send(reply.encode())
		elif code == self.DETAIL_200:
			width, length = self.clientInfo['videoStream'].getSize()
			fps = self.clientInfo['videoStream'].getFPS()
			frameNum = self.clientInfo['videoStream'].getFramNum()
			reply = 'RTSP/1.0 200 OK\nCSeq: ' + seq + '\nSession: ' + str(self.clientInfo['session']) + '\n' + str(width) + " " + str(length) + " " + str(fps) + " " + str(frameNum)
			print("reply detail: ",reply)
			connSocket = self.clientInfo['rtspSocket'][0]
			connSocket.send(reply.encode())
		# Error messages
		elif code == self.FILE_NOT_FOUND_404:
			print("404 NOT FOUND")
		elif code == self.CON_ERR_500:
			print("500 CONNECTION ERROR")	


if __name__ == "__main__":
	try:
		SERVER_PORT = int(sys.argv[1])
	except:
		print("[Usage: Server.py Server_port]\n")
	rtspSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	rtspSocket.bind(('', SERVER_PORT))
	rtspSocket.listen(5)        
	fileSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	fileSocket.bind(('', 12345))
	fileSocket.listen(5)

	# Receive client info (address,port) through RTSP/TCP session
	while True:
		clientInfo = {}
		clientInfo['rtspSocket'] = rtspSocket.accept()
		clientInfo['fileSocket'] = fileSocket.accept()
		Server(clientInfo).run()	

