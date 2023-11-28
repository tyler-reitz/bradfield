import socket

sock = socket.socket()

sock.bind(('', 64998))

sock.listen()

while True:
    s, a = sock.accept()

    while True:
        data = s.recv(4096)
        if data == b'' or data == b'\n': break

    s.close()

sock.close()
