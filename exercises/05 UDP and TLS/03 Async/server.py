from socket import socket, SOCK_DGRAM, AF_INET
from calculator import process
import asyncio

"""
Using asyncio of course
Client is the same as in first task, look in ../udp
"""

async def handle(sock, data, remote):
    expression = str(data, encoding='UTF-8')
    try:
        result = process(expression)
        print('Resolved', expression.rstrip(), 'to', result)
        sock.sendto(bytes('= ' + str(result) + '\n', encoding='UTF-8'), remote)
    except Exception as error:
        result = str(error)
        sock.sendto(bytes('ERROR: ' + str(error) + '\n', encoding='UTF-8'), remote)

ADDRESS = ('', 8080)

async def main():
    with socket(AF_INET, SOCK_DGRAM) as sock:
        sock.bind(ADDRESS)
        while True:
            # recvfrom is a blocking operation
            data, remote = await loop.run_in_executor(None, lambda: sock.recvfrom(1024))
            loop.create_task(handle(sock, data, remote))

loop = asyncio.get_event_loop()
loop.run_until_complete(main())
        