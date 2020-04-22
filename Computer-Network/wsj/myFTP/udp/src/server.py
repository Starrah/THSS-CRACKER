import socket

size = 8192

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind(('', 9876))
count = 0

try:
  while True:
    data, address = sock.recvfrom(size)
    sock.sendto(bytes(str(count)+" "+str(data,encoding='utf-8'),encoding='utf-8'), address)
    count += 1
finally:
  sock.close()