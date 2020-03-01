import socket
from calculator import process

ADDRESS = ('', 8080)

with socket.create_server(ADDRESS) as server:
    server.listen(1)
    (connection, remote) = server.accept()
    with connection:
        connection.send(b'Please input a RPN expression\n> ')
        while True:
            data = connection.recv(1024)
            if not data: break
            expression = str(data, encoding='UTF-8')
            if expression == '': break
            try:
                result = process(expression)
                print('Resolved', expression.rstrip(), 'to', result)
                connection.send(bytes('= ' + str(result) + '\n> ', encoding='UTF-8'))
            except Exception as error:
                result = str(error)
                connection.send(bytes('ERROR: ' + str(error) + '\n> ', encoding='UTF-8'))

        connection.send(b'See you later alligator\n')
