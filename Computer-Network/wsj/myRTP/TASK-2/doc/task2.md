###1. 运行环境

```
python == 3.6
pyqt5 == 5.10
opencv-python == 4.1.1.26
```



###2. 启动方法

```
请按照以下格式先启动server再启动client，注意以下下列启动代码都是相对各自src路径下
server: python Server.py serverPort

client: python Client.py 127.0.0.1 serverPort rtpPort
```

运行示例：

```
python Server.py 1024

python Client.py 127.0.0.1 1024 1025
```



###4. 运行说明：

1.进入界面后，最右侧会自动加载出服务器src/video目录下的所有视频文件，单机选中其中一项，点击setup则载入视频；若不选则默认载入第一个视频。

2.setup载入后点击play按键开始播放，点击pause可以暂停。也可以通过空格键切换暂停播放，通过左右方向键调整进度，也可以拖动进度条。

3.点击下面三个按钮可以切换播放速度。

4.点击Teardown会停止当前播放并回到最初未载入的状态，此时再次setup可以载入其他选中视频。

5.切换视频的操作流程：Teardown -> 播放列表选中播放视频 -> Setup -> Play ->Teardown -> ....

6.播放视频必须放在server的src/video目录下

7.客户端字幕文件会根据视频名自动加载，应放在Client.py的同一目录下。

![image-20191211214959137](https://tva1.sinaimg.cn/large/006tNbRwly1g9t4f7ir1dj31l40u07wh.jpg)



###5. TASK2介绍

详情见根目录`report.pdf`
