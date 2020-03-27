from socket import socket, SOCK_DGRAM, AF_INET
# import sys
# sys.path.append('../04 TCP Sockets')
from calculator import process
import threading

def handle(data, remote):
    expression = str(data, encoding='UTF-8')
    try:
        result = process(expression)
        print('Resolved', expression.rstrip(), 'to', result)
        socket.sendto(bytes('= ' + str(result) + '\n', encoding='UTF-8'), remote)
    except Exception as error:
        result = str(error)
        socket.sendto(bytes('ERROR: ' + str(error) + '\n', encoding='UTF-8'), remote)

ADDRESS = ('', 8080)

with socket(AF_INET, SOCK_DGRAM) as socket:
    socket.bind(ADDRESS)
    while True:
        data, remote = socket.recvfrom(1024)
        threading.Thread(None, handle, args=(data, remote)).start()
        