'use strict';

const net = require('net');
const fs = require('fs');
const ip = require('ip');
const WebSocket = require('./whereissock.js');
// insert local IP into the HTML's embedded script
const CONTENT = fs.readFileSync('./index.html').toString()
  .replace('localhost', ip.address());
const RESPONSE = 'HTTP/1.1 200 OK\r\nContent-Length: ' + CONTENT.length + '\r\n\r\n' + CONTENT;

// Simple HTTP server responds with a simple WebSocket drawing board
const httpServer = net.createServer(connection => {
  connection.on('data', () => {
    connection.write(RESPONSE);
  });
});

httpServer.listen(3000, () => {
  console.log('HTTP server listening on port 3000');
});

const ws = new WebSocket();

// remember what is drawn
let drawn = [];
const SIZE = 32;
for (let i = 0; i < SIZE; i++) {
  drawn[i] = [];
  for (let j = 0; j < SIZE; j++) {
    drawn[i][j] = false;
  }
}

ws.listen(3001, () => console.log('we\'re rolling'));

// add message handler
ws.onmessage = message => {
  const cmd = message.split(' ');
  if (cmd.length === 3 && cmd[0] === 'DRAW') {
    // if drawing message
    const [x, y] = cmd.slice(1,3);
    if (drawn[x][y] == false) {
      // only draw if not drawn!
      drawn[x][y] = true;
      ws.broadcast(message);
    }
  } else {
    ws.broadcast(message);
  }
}

// just a test for >125 bytes long message from server
//setTimeout(() => ws.broadcast('aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa'), 8000);

