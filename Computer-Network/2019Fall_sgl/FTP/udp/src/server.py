import socket

size = 8192

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind(('', 9876))
seq = 1
try:
	while True:
		data, address = sock.recvfrom(size)
		msg_str = data.upper().decode()
		msg = (str(seq) + ' ' + msg_str).encode()
		sock.sendto(msg, address)
		seq = seq + 1
finally:
	sock.close()