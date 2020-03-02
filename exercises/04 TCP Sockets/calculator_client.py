import sys
import socket

if len(sys.argv) > 1 and sys.argv[1]:
    ADDRESS = ('', int(sys.argv[1]))
else:
    ADDRESS = ('', 8080)

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
    sock.connect(ADDRESS)
    print(str(sock.recv(1024), 'UTF-8'), end='')
    while True:
        expression = input() # Prompted by server
        if expression == '': break
        sock.send(bytes(expression, 'UTF-8'))
        response = str(sock.recv(1024), 'UTF-8')
        print(response, end='')
