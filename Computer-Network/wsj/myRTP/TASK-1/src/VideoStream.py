import os
class VideoStream:
    def __init__(self, foldername):
        self.foldername = foldername
        self.frameNum = 0
        self.imgNum = len(os.listdir(self.foldername))
    def nextFrame(self):
        """Get next frame."""
        filename =  "%s/%04d.jpg" %(self.foldername,self.frameNum)
        try:
            img = open(filename,'rb')
            if img:            
                # Read the current frame
                data = img.read()
                self.frameNum += 1
            return data
        except:
            return None
        
    def frameNbr(self):
        """Get frame number."""
        return self.frameNum
