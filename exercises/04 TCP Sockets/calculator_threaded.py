import socket
import threading
import socketserver
from calculator import process

class ThreadedCalculationHandler(socketserver.BaseRequestHandler):

    def handle(self):
        self.request.send(b'Please input a RPN expression\n> ')
        print('New connection, serving from {}'.format(threading.current_thread().native_id))

        while True:
            expression = str(self.request.recv(1024), 'UTF-8').strip()
            if expression == '': break
            try:
                result = process(expression)
                print('Resolved', expression.rstrip(), 'to', result)
                result = '= ' + str(result) + '\n> '
            except Exception as error:
                result = 'ERROR: ' + str(error) + '\n> '

            self.request.send(bytes(result, 'UTF-8'))

        self.request.send(b'See you later alligator\n')

class ThreadedTCPServer(socketserver.ThreadingMixIn, socketserver.TCPServer):
    pass

if __name__ == '__main__':
    # 0 = random port
    HOST, PORT = 'localhost', 0

    server = ThreadedTCPServer((HOST, PORT), ThreadedCalculationHandler)
    with server:
        ip, port = server.server_address
        print(f'Serving at {port}')
        server.serve_forever()
