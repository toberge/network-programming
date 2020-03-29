import sys
import socket

ADDRESS = ('', 8080)
if len(sys.argv) == 2 and sys.argv[1]:
    ADDRESS = ('', int(sys.argv[1]))
elif len(sys.argv) == 3 and sys.argv[1] and sys.argv[2]:
    ADDRESS = (sys.argv[1], int(sys.argv[2]))

with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as sock:
    sock.connect(ADDRESS)
    while True:
        expression = input('> ') # Prompted by server
        if expression == '': break
        sock.send(bytes(expression, 'UTF-8'))
        response = str(sock.recv(1024), 'UTF-8')
        print(response, end='')
