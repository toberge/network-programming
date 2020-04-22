from socket import socket, SOCK_DGRAM, AF_INET
from pickled_calculator import process
import pickle
import threading

def handle(data, remote):
    expression = pickle.loads(data)
    try:
        result = process(expression)
        print('Resolved', expression, 'to', result)
        socket.sendto(pickle.dumps(result), remote)
    except Exception as error:
        socket.sendto(pickle.dumps({'error': str(error)}), remote)

ADDRESS = ('', 8080)

with socket(AF_INET, SOCK_DGRAM) as socket:
    socket.bind(ADDRESS)
    while True:
        data, remote = socket.recvfrom(1024)
        threading.Thread(None, handle, args=(data, remote)).start()
