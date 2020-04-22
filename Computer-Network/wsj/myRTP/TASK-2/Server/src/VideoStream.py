import os
import numpy as np
import cv2

class VideoStream:
    def __init__(self, videoName):
        self.videoName = videoName
        self.frameNum = 0
        self.videoCapture = cv2.VideoCapture(self.videoName)
        #获得码率及尺寸
        self.fps = int(self.videoCapture.get(cv2.CAP_PROP_FPS))
        self.size = (int(self.videoCapture.get(cv2.CAP_PROP_FRAME_WIDTH)), 
                int(self.videoCapture.get(cv2.CAP_PROP_FRAME_HEIGHT)))
        self.fNUMS = int(self.videoCapture.get(cv2.CAP_PROP_FRAME_COUNT))
        self.framePos = 0
        self.setPos = False

    def nextFrame(self, framePos):
        """Get next frame."""
        if self.setPos:
            self.videoCapture.set(cv2.CAP_PROP_POS_FRAMES, framePos)
            self.setPos = False
            self.frameNum = framePos
        success, frame = self.videoCapture.read()
        if success :
            self.frameNum += 1
            frame = np.array(frame)
            q = [int(cv2.IMWRITE_JPEG_QUALITY), 50]
            ret, buffer = cv2.imencode('.jpg', frame, q)
            data = buffer.tobytes()
            return data
        else:
            return None
    #def setPos(self, framePos):
    #    self.videoCapture.set(cv2.CAP_PROP_POS_FRAMES, framePos)

    def getSize(self):
        return self.size

    def getFPS(self):
        return self.fps
    
    def getFramNum(self):
        return self.fNUMS

    def frameNbr(self):
        """Get frame number."""
        return self.frameNum