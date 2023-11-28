# Shout back server
import socket

s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) 
s.bind(('', 7777))

while True:
    data, addr = s.recvfrom(1024)
    s.sendto(data.upper(), addr)

s.close()
