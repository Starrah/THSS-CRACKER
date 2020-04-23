###1. 启动方法

```
请按照以下格式先启动server再启动client
server: python Server.py serverPort

client: python Client.py 127.0.0.1 serverPort rtpPort imgsPath
```

运行示例：

```
python Server.py 1024

python Client.py 127.0.0.1 1024 1025 imgs
```



###2. TASK1简介

task1在给定代码基础上完善并完成RTP协议以及客户端与服务端，实现传输图片流并播放的功能，实现后界面如下：

![image-20191211093630520](https://tva1.sinaimg.cn/large/006tNbRwly1g9sj811jfmj31iq0u0txx.jpg)

task1代码结构如下：其中imgs文件夹中为测试图片文件夹。实现了播放/暂停视频的基本功能，SETUP，PLAY，PAUSE，TEARDOWN这四个RTSP命令。

![image-20191211093953140](https://tva1.sinaimg.cn/large/006tNbRwly1g9sjbjamyzj307u05kdfx.jpg)

下面介绍server的思路：

Server.py中定义了Server类，其具有INIT，READY，PLAYING这三种状态，当服务器运行时，将新创建一个进程recvRtspRequest来监听服务器的RTSP命令以防止阻塞主进程，当收到RTSP命令后解析并根据命令执行相关操作。服务器的状态转移图如下：

![image-20191211094656077](https://tva1.sinaimg.cn/large/006tNbRwly1g9sjivgxooj31a00hw10l.jpg)

在setup后，服务器会建立起RTP套接字用于传输流数据，在任务一中我采用了每次传输一张图片作为视频一帧的方法，每隔0.05秒传输一次，client在接收到图片后立刻渲染，这样客户端可以播放FPS=20的视频。







