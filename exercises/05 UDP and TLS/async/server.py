from socket import socket, SOCK_DGRAM, AF_INET
# import sys
# sys.path.append('../04 TCP Sockets')
from calculator import process
import asyncio
import functools

async def handle(sock, data, remote):
    expression = str(data, encoding='UTF-8')
        # if expression == '': continue
    try:
        result = process(expression)
        print('Resolved', expression.rstrip(), 'to', result)
        sock.sendto(bytes('= ' + str(result) + '\n', encoding='UTF-8'), remote)
    except Exception as error:
        result = str(error)
        sock.sendto(bytes('ERROR: ' + str(error) + '\n', encoding='UTF-8'), remote)

ADDRESS = ('', 8080)

# From https://stackoverflow.com/questions/41063331/how-to-use-asyncio-with-existing-blocking-library
# TODO: write yer own thing, yo...
def run_in_executor(f):
    @functools.wraps(f)
    def inner(*args, **kwargs):
        loop = asyncio.get_running_loop()
        return loop.run_in_executor(None, lambda: f(*args, **kwargs))
    return inner

@run_in_executor
def recvfrom(sock, num):
    return sock.recvfrom(num)

async def main():
    with socket(AF_INET, SOCK_DGRAM) as sock:
        #sock.setblocking(False)
        sock.bind(ADDRESS)
        while True:
            data, remote = await recvfrom(sock, 1024)
            # but recvfrom() is blocking...
            loop.create_task(handle(sock, data, remote))

loop = asyncio.get_event_loop()
loop.run_until_complete(main())
        