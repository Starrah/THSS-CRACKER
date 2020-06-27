首先，两个客户端都bind到某个确定的端口。

然后一个客户端先发消息，然后进入接收模式。

另一个客户端先接收，然后回复消息。

第一个客户端：

```
bind(port1)

while(1):

​	send(Message, port2)

​	Reply = receive()
```

第二个客户端：

```
bind(port2)

while(1):

​	Reply = receive()

​	send(Message,port1)
```

这样就能实现两个客户端的持续通信了。