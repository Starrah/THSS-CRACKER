文件结构：

RtpPacket.py：RTP数据包。

Constants.py常量。

MainServer.py和MainServerManager.py:主服务器 

RTPServer.py和RTPServerManager.py:RTP服务器

依赖库：pyqt5,tkinter，PIL，opencv（cv2）
使用环境：开发在ubuntu16.04下，只支持linux系统
使用方法：必须在同一个ip下运行两个服务器：

python RTPServer.py 

python MainServer.py

要求：两个server的对应文件都在同一个目录下，这个目录下有ServerDir文件夹，里面存放着待播放视频（暂时只支持avi,mp4,flv几个格式）和字幕文件（只支持src格式，必须和视频文件同名，例子有提供）



