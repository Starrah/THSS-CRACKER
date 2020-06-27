文件结构：

Constants.py：常量

RtpPacket.py：RTP包

两个ui：pyqt主客户端用

Client.py:pyqt主客户端

PlayClient.py:播放器tkinter客户端

依赖库：pyqt5,tkinter，PIL，opencv（cv2）
使用环境：开发在ubuntu16.04下，只支持linux系统
使用方法：保证两个server运行在同一个ip下而且都在运行的前提下使用。

登录时输入服务器ip即可。

之后进入主界面，可以查看和搜索视频，点击单一视频能获得视频文件信息，右键进入播放。

进入播放后会自动隐藏主界面。然后左上角关闭播放界面后会回到主界面。

注意，搜索框的中文输入法可能被屏蔽，但是复制粘贴可以进行。搜索中文最好复制粘贴。