import socket
import threading
import socketserver
from calculator import process



ADDRESS = ('192.168.2.43', 80)

START = """HTTP/1.0 200 OK
Content-Type: text/html; charset=utf-8

<html>
<body>
<h1> Hello there </h1>
<ul>
"""
END = """
</ul>
<br><br>
<p>w̶̳̰̤̳͈̫̻̜̺͉͓͖̮̘̜̌̾͑̄̓̽̑̐̕ȩ̸͙͔̺͒͋͋̎̏̉̑̐͊̏͗́̓̋̚ ̴̻̖̦̹̦̄͂͆̀̈͐̂̈͒̅̐̕͝͠s̵̡̹̫̬͕̬̣̰͓͍̜͈̖̪͕̱͒͘̚ȩ̷̧̧̛͚̩̻̱̺̜̰͍̻͕̪͂͋̿͊̊̀͊͋͊͛̔͌̋͛̕̕͝ẽ̶̈́̊̿̋̌̾͐͂̉́͂̀͜ ̸̨̬̻̗̪̊̑̎́̃̀̿́̓̔̋̔͒̆̚͝͝ȳ̴̧̭̗̺͎͈̝̲̘̖̩̖͙̻̘̝̅́̓̌̽̽ȯ̸̜̬̰̙̈́͛̍̀͂͗̂̍͠͠ȕ̸̧̢̡̨̩̗̩̫̫̟̟̞̗̭̫̬ͅͅ</p>
<img src="https://i.ya-webdesign.com/images/eye-silhouette-png-7.png" alt="ahaha" width="100px">
<p style="color: white; border: dotted #ccc 1px;">aaa it burns aaa it burns aaa it burns aaa it burns aaa it burns aaa it burns aaa it burns aaa it burns aaa it burns</p>
</body>
</html>
"""

def assemble(headers):
    res = START
    for header in headers:
        res += f'<li>{header}</li>\n'
    res += END
    return bytes(res, encoding='UTF-8')

class HTTPRequestHandler(socketserver.BaseRequestHandler):

    def handle(self):
        plaintext = str(self.request.recv(1024), encoding='UTF-8')
        print('-------------------')
        print('Serving from {}'.format(threading.current_thread().name))
        print(plaintext)
        headers = plaintext.splitlines()
        if headers[0].startswith('GET / HTTP/1.1'):
            self.request.sendall(assemble(headers[1:-1]))
        else:
            self.request.sendall(b'HTTP/1.0 404 ERROR\n\n<h1>404 Not Found</h1>')

class ThreadedTCPServer(socketserver.ThreadingMixIn, socketserver.TCPServer):
    pass

if __name__ == '__main__':
    server = ThreadedTCPServer(ADDRESS, HTTPRequestHandler)
    with server:
        ip, port = server.server_address
        print(f'Serving at {port}')
        server.serve_forever()

# curl localhost:8080 -H "hi_there:<script>alert('aaa it burns')</script>"
