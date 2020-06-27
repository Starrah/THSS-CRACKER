## 1.Server配置和使用方法

**首先，因为21接口需要权限，以下操作必须在root下执行。**

server可执行文件已经在server/src文件夹中了，./server 加上参数就可以连接了

也可以在server/src文件夹里输入以下gcc指令生成server

`gcc RequestHandle.h RequestHandle.c StatusChange.h PathHandle.c PathHandle.h Server.c SocketHandle.c SocketHandle.h StatusChange.h StatusChange.c -o server -lpthread `

将autograde.py和server放到一起就可以测试，但是**一定不能把他们都放在/tmp文件夹里**，否则会出现错误结果。

## 2.Client配置和使用方法

在client/dist/Client文件夹里的Client文件就是可执行文件，可以在命令行里输入./Client打开。

首先是登录界面，在这里输入好后点击登录即可。如果连接自己的服务器，那么只支持anonymous登录。

![login](/home/sercharles/文档/FTP/doc/login.jpeg)

之后是主界面

![mainwindow](/home/sercharles/文档/FTP/doc/mainwindow.jpeg)

主界面左上方显示着当前服务器情况，用户名和当前服务器工作目录。右上角的按钮顾名思义，Disconnect是断开连接并退出（不能用右上角的叉退出），GoTo是前往目录框的位置，Back是回退，MainMenu是回到服务器默认目录，New是新建文件夹（会出现输入框输入），Upload是上传文件到服务器（会出现文件选择框选择），Refresh是刷新文件列表。

![info](/home/sercharles/文档/FTP/doc/info.jpeg)

![buttons](/home/sercharles/文档/FTP/doc/buttons.jpeg)

下方的框是文件列表，显示文件名和相应信息。选择某个文件后，右击会出现一个菜单，可以进入（文件夹），删除（空文件夹），重命名，下载（文件）。

再下方的Download和Upload分别是下载，上传列表。STOP按钮是结束传输，PAUSE/RESUME按钮是暂停，继续（只有连接自己的服务器才能继续传输），右面有进度条。只能同时进行一个文件的下载和传输。下载/传输完毕会有提醒。

![loadlist](/home/sercharles/文档/FTP/doc/loadlist.jpeg)