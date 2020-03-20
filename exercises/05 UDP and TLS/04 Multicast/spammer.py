import socket
import os
import sys
from time import sleep

"""

"""

copypastas = []

# Read all copypastas from folder
for i, entry in enumerate(os.listdir('copypasta')):
    with open(f'{os.getcwd()}/copypasta/{entry}') as file:
        copypastas.append([])
        for line in file:
            copypastas[i].append(line)

print(copypastas[0][0])

# group and port
# localhost if running locally
if len(sys.argv) == 2 and sys.argv[1] == 'local':
    ADDRESS = ('localhost', 6969)
    print('Spamming loopback')
else:
    print('Spamming internal network')
    ADDRESS = ('224.3.29.71', 6969)

# UDP socket
with socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP) as sock:
    # Set TTL
    sock.setsockopt(socket.IPPROTO_IP, socket.IP_MULTICAST_TTL, 3)

    sock.sendto(b'I\'ll have you know...', ADDRESS)

    while True:
        # Send spam
        for copypasta in copypastas:
            for line in copypasta:
                sock.sendto(bytes(line, encoding='UTF-8'), ADDRESS)
                print('.', end='')
                # Sleep based on length of text
                # One second per 35 chars
                sleep(len(line) / 35)
            print()
