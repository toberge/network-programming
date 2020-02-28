import socket

ADDRESS = ('', 8080)

# can check socket.has_dualstack_ipv6()
# and add family=socket.AF_INET6, dualstack_ipv6=True
# if it is true
with socket.create_server(ADDRESS) as server:
    (connection, remote) = server.accept()
    #print(remote) # IP and assigned port?
    with connection:

        #connection.send(bytes('hi there\n', encoding='UTF-8'))
        connection.send(b'Hello from server.py\n')
        print(str(connection.recv(1024), encoding='UTF-8'))
        connection.send(b'bye fool\n')

    # connection.close()
