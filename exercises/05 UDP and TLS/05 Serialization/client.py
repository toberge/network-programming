import sys
import socket
from pickle import dumps, loads

if len(sys.argv) == 2 and sys.argv[1]:
    ADDRESS = ('', int(sys.argv[1]))
elif len(sys.argv) == 3 and sys.argv[1] and sys.argv[2]:
    ADDRESS = (sys.argv[1], int(sys.argv[2]))
else:
    ADDRESS = ('', 8080)

OPERATORS = set('-+*/')

with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as sock:
    sock.connect(ADDRESS)
    while True:
        expression = input('> ')
        if expression == '': break
        # parse integers, leave operators alone
        try:
            parsed = []
            for s in expression.split():
                if s in OPERATORS:
                    parsed.append(s)
                else:
                    parsed.append(float(s))
        # abort if not a number
        except ValueError as error:
            print(str(error))
            print('Aborting...')
            break
            
        # serialize and send
        sock.send(dumps(parsed))
        response = loads(sock.recv(1024))
        print(response)
