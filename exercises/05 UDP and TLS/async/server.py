from socket import socket, SOCK_DGRAM, AF_INET
# import sys
# sys.path.append('../04 TCP Sockets')
from calculator import process
import asyncio

async def handle(data, remote):
    expression = str(data, encoding='UTF-8')
        # if expression == '': continue
    try:
        result = process(expression)
        print('Resolved', expression.rstrip(), 'to', result)
        socket.sendto(bytes('= ' + str(result) + '\n', encoding='UTF-8'), remote)
    except Exception as error:
        result = str(error)
        socket.sendto(bytes('ERROR: ' + str(error) + '\n', encoding='UTF-8'), remote)

ADDRESS = ('', 8080)

async def main():
    with socket(AF_INET, SOCK_DGRAM) as sock:
        #sock.setblocking(False)
        sock.bind(ADDRESS)
        while True:
            data, remote = sock.recvfrom(1024)
            # but recvfrom() is blocking...
            loop.create_task(handle(data, remote))

loop = asyncio.get_event_loop()
loop.run_until_complete(main())
        