文件结构：

RtpPacket.py：RTP数据包。

Constants.py常量。

Server.py和ServerManager.py:服务器 

Client.py：客户端

依赖库：pyqt5,tkinter，PIL，opencv（cv2）
使用环境：开发在ubuntu16.04下，只支持linux系统
使用方法：

python Server.py

python Client.py

分别打开服务器和客户端，要求这两个在同一ip下运行，然后客户端依次用setup，play按钮播放test文件夹里的图片，pause指令还没在这里被支持，但是可以teardown。



