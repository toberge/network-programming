import socket

ADDRESS = ('', 8080)

START = """HTTP/1.0 200 OK
Content-Type: text/html; charset=utf-8

<html>
<body>
<h1> Hello there </h1>
<ul>
"""
END = """
</ul>
</body>
</html>
"""

def assemble(headers):
    res = START
    for header in headers:
        res += f'<li>{header}</li>\n'
    res += END
    return bytes(res, encoding='UTF-8')

with socket.create_server(ADDRESS) as server:
    server.listen(1)
    (connection, remote) = server.accept()
    with connection:
        #while True:
        data = connection.recv(1024)
        plaintext = str(data, encoding='UTF-8')
        print(plaintext)
        headers = plaintext.splitlines()
        if headers[0].startswith('GET / HTTP/1.1'):
            connection.sendall(assemble(headers[1:-1]))

# curl localhost:8080 -H "hi_there:<script>alert('aaa it burns')</script>"
