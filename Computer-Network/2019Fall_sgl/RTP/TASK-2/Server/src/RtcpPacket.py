import sys
from time import time
import datetime
import ntplib
HEADER_SIZE = 28
BLOCK_SIZE = 24

class RtcpPacket:	
	header = bytearray(HEADER_SIZE)
	
	def __init__(self):
		pass
		
	def encode(self, version, padding, reportCount, packetType, length, ssrc\
	, timestampRtp, packetCount, octetCount, reportBlockList, payload):
		"""Encode the RTP packet with header fields and payload."""
		
		timestampNtpHigh, timestampNtpLow = self.getNtpTime()
		#print(timestampNtpHigh, timestampNtpLow)
		header = bytearray(HEADER_SIZE)
		self.reportBlocks = []
		
		# Fill the header bytearray with RTP header fields
		header[0] = (version << 6) | (padding << 5) | reportCount
		header[1] = packetType
		#length
		header[2] = (length >> 8) & 255 #upper bits
		header[3] = length & 255
		#ssrc
		header[4] = (ssrc >> 24) & 255
		header[5] = (ssrc >> 16) & 255
		header[6] = (ssrc >> 8) & 255
		header[7] = (ssrc) & 255

		#ntp timestamp
		header[8] = (timestampNtpHigh >> 24) & 255
		header[9] = (timestampNtpHigh >> 16) & 255
		header[10] = (timestampNtpHigh >> 8) & 255
		header[11] = (timestampNtpHigh) & 255
		header[12] = (timestampNtpLow >> 24) & 255
		header[13] = (timestampNtpLow >> 16) & 255
		header[14] = (timestampNtpLow >> 8) & 255
		header[15] = (timestampNtpLow) & 255
		
		#rtp timestamp
		header[16] = (timestampRtp >> 24) & 255
		header[17] = (timestampRtp >> 16) & 255
		header[18] = (timestampRtp >> 8) & 255
		header[19] = (timestampRtp) & 255

		#packetcount
		header[20] = (packetCount >> 24) & 255
		header[21] = (packetCount >> 16) & 255
		header[22] = (packetCount >> 8) & 255
		header[23] = (packetCount) & 255

		#octetcount
		header[24] = (octetCount >> 24) & 255
		header[25] = (octetCount >> 16) & 255
		header[26] = (octetCount >> 8) & 255
		header[27] = (octetCount) & 255
		self.header = header

		for i in range(reportCount):
			theReport = bytearray(BLOCK_SIZE)
			theReport[0] = (reportBlockList[i]["ssrc"] >> 24) & 255
			theReport[1] = (reportBlockList[i]["ssrc"] >> 16) & 255
			theReport[2] = (reportBlockList[i]["ssrc"] >> 8) & 255
			theReport[3] = (reportBlockList[i]["ssrc"]) & 255

			theReport[4] = reportBlockList[i]["fractionLost"]
			theReport[5] = (reportBlockList[i]["packetLost"] >> 16) & 255
			theReport[6] = (reportBlockList[i]["packetLost"] >> 8) & 255
			theReport[7] = (reportBlockList[i]["packetLost"]) & 255

			theReport[8] = (reportBlockList[i]["sequenceNum"] >> 24) & 255
			theReport[9] = (reportBlockList[i]["sequenceNum"] >> 16) & 255
			theReport[10] = (reportBlockList[i]["sequenceNum"] >> 8) & 255
			theReport[11] = (reportBlockList[i]["sequenceNum"]) & 255

			theReport[12] = (reportBlockList[i]["jitter"] >> 24) & 255
			theReport[13] = (reportBlockList[i]["jitter"] >> 16) & 255
			theReport[14] = (reportBlockList[i]["jitter"] >> 8) & 255
			theReport[15] = (reportBlockList[i]["jitter"]) & 255

			theReport[16] = (reportBlockList[i]["lsr"] >> 24) & 255
			theReport[17] = (reportBlockList[i]["lsr"] >> 16) & 255
			theReport[18] = (reportBlockList[i]["lsr"] >> 8) & 255
			theReport[19] = (reportBlockList[i]["lsr"]) & 255

			theReport[20] = (reportBlockList[i]["dlsr"] >> 24) & 255
			theReport[21] = (reportBlockList[i]["dlsr"] >> 16) & 255
			theReport[22] = (reportBlockList[i]["dlsr"] >> 8) & 255
			theReport[23] = (reportBlockList[i]["dlsr"]) & 255
			self.reportBlocks.append(theReport)

		# Get the payload from the argument
		self.payload = payload
		
	def decode(self, byteStream):
		"""Decode the RTP packet."""
		self.header = bytearray(byteStream[:HEADER_SIZE])
		theReportCount = self.reportCount()
		for i in range(theReportCount):
			theBlock = bytearray(byteStream[HEADER_SIZE + i * BLOCK_SIZE: HEADER_SIZE + (i + 1) * BLOCK_SIZE])
			self.reportBlocks.append(theBlock)
		self.payload = byteStream[HEADER_SIZE + theReportCount * BLOCK_SIZE:]
	
	def getNtpTime(self):
		c = ntplib.NTPClient()
		response = c.request('europe.pool.ntp.org', version=3)
		theHigh = int(str(response.tx_time).split('.')[0])
		theLow = int(str(response.tx_time).split('.')[1])
		return theHigh, theLow

	def version(self):
		"""Return RTP version."""
		return int(self.header[0] >> 6)
	
	def padding(self):
		return int((self.header[0] >> 5) & 1) 

	def reportCount(self):
		return int(self.header[0] & 31)

	def packetType(self):
		return int(self.header[1])

	def length(self):
		return int(self.header[2] << 8 | self.header[3])
	
	def ssrc(self):
		return int(self.header[4] << 24 | self.header[5] << 16 | self.header[6] << 8 | self.header[7])
	
	def timestampNtpHigh(self):
		return int(self.header[8] << 24 | self.header[9] << 16 | self.header[10] << 8 | self.header[11])

	def timestampNtpLow(self):
		return int(self.header[12] << 24 | self.header[13] << 16 | self.header[14] << 8 | self.header[15])

	def timestampRtp(self):
		return int(self.header[16] << 24 | self.header[17] << 16 | self.header[18] << 8 | self.header[19])	

	def packetCount(self):
		return int(self.header[20] << 24 | self.header[21] << 16 | self.header[22] << 8 | self.header[23])

	def octetCount(self):	
		return int(self.header[24] << 24 | self.header[25] << 16 | self.header[26] << 8 | self.header[27])

	def getReportBlock(self, i):
		TheBlock = {}
		TheBlock["ssrc"] = int(self.reportBlocks[i][0] << 24 | self.reportBlocks[i][1] << 16 | self.reportBlocks[i][2] << 8 |self.reportBlocks[i][3])
		TheBlock["fractionLost"] = int(self.reportBlocks[i][4])
		TheBlock["packetLost"] = int(self.reportBlocks[i][5] << 16 + self.reportBlocks[i][6] << 8 + self.reportBlocks[i][7])
		TheBlock["sequenceNum"] = int(self.reportBlocks[i][8] << 24 | self.reportBlocks[i][9] << 16 | self.reportBlocks[i][10] << 8 |self.reportBlocks[i][11])
		TheBlock["jitter"] = int(self.reportBlocks[i][12] << 24 | self.reportBlocks[i][13] << 16 | self.reportBlocks[i][14] << 8 |self.reportBlocks[i][15])
		TheBlock["lsr"] = int(self.reportBlocks[i][16] << 24 | self.reportBlocks[i][17] << 16 | self.reportBlocks[i][18] << 8 |self.reportBlocks[i][19])
		TheBlock["dlsr"] = int(self.reportBlocks[i][20] << 24 | self.reportBlocks[i][21] << 16 | self.reportBlocks[i][22] << 8 |self.reportBlocks[i][23])
		return TheBlock
		
	def getPayload(self):
		"""Return payload."""
		return self.payload
		
	def getPacket(self):
		"""Return RTP packet."""
		thePacket = self.header
		theReportCount = self.reportCount()
		for i in range(theReportCount):
			thePacket = thePacket + self.reportBlocks[i]
		return thePacket + self.payload

