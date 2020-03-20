import socket
import struct

"""
Based on https://stackoverflow.com/a/1794373
which is based on https://wiki.python.org/moin/UdpCommunication#Multicasting.3F
which doesn't quite work...

The server must send to localhost if both run on loopback.
"""

# group and port
GROUP = '224.3.29.71'
ADDRESS = ('0.0.0.0', 6969)

# Creating UDP socket
with socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP) as sock:
    # Set option
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    # Listen only on this group
    sock.bind(ADDRESS)
    # Create membership request struct
    mreq = struct.pack('4sl', socket.inet_aton(GROUP), socket.INADDR_ANY)
    # Register membership in multicast group
    sock.setsockopt(socket.IPPROTO_IP, socket.IP_ADD_MEMBERSHIP, mreq)

    while True:
        print(str(sock.recv(1024), 'UTF-8').rstrip())
