import socket
 
size = 8192
 
try:
  sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
  for i in range(0,51):
    msg = bytes(str(i), encoding="utf-8")
    sock.sendto(msg, ('127.0.0.1', 9876))
    print(str(sock.recv(size),encoding='utf-8'))
  sock.close()
 
except:
  print("cannot reach the server")