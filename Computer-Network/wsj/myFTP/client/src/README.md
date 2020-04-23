FTP client：

使用python完成了命令行客户端和GUI客户端。

启动命令行客户端：

```
python client.py ${ip} ${client}
例如：
python client.py 127.0.0.1 21
也可以直接：
python client.py  此时IP和端口是默认值
```

命令行客户端支持命令：

```
["USER","PASS","STOR","RETR","QUIT","SYST","TYPE","PORT",
        "PASV","LIST","REST","RNFR","RNTO","MKD","CWD","PWD","RMD",
        "login","ls","upload","download","mv","pwd"]
```



启动GUI客户端：

```
python gui_client.py
```

python=3.7

pyqt=5.10