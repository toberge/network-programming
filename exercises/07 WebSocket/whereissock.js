'use strict';

const net = require('net');
const base64 = require('crypto-js/enc-base64');
const SHA1 = require('crypto-js/sha1');

const STANDARD_CONSTANT = '258EAFA5-E914-47DA-95CA-C5AB0DC85B11'

/**
 * wereissock.js - a simple WebSocket library
 */
module.exports = class WebSocket {
    constructor() {
        this.server = net.createServer(connection => {

            // once chance to handshake
            connection.once('data', data => {
                this.__handleHandshake(data, connection);
            });

            connection.on('error', this.onerror)

            connection.on('end', this.ondisconnect);
        });
        this.clients = [];
    };

    /**
     * Form HTTP 101 response for completing ws handshake
     * Note that there must be two newlines AT THE END
     * of the HTTP headers, separating them from possible payload.
     * (my mistake was putting two newlines after the response type)
     */
    __formHeader = key => {
        const encoded = base64.stringify(SHA1(key + STANDARD_CONSTANT))
        return `HTTP/1.1 101 Switching Protocols
Upgrade: websocket
Connection: Upgrade
Sec-WebSocket-Accept: ${encoded}

`
    }


    /**
     * Only once, accept handshake and deny anything else.
     */
    __handleHandshake = (data, connection) => {
        let strings = data.toString().split('\r\n');
        
        if (strings[0] === 'GET / HTTP/1.1') {
            console.log('Incoming connection!');
            // after handshake
            strings = strings.slice(1);
            let key = null;
            let valid = true;

            // filter key from headers
            for (const string of strings) {
                const [header, value] = string.split(': ');
                if (header === 'Sec-WebSocket-Key') {
                    key = value;
                } else if (header === 'Connection' && value == 'Upgrade') {
                    valid = true;
                }
            }

            // fail if no key or invalid header set
            if (!key || !valid) {
                this.onerror('No key or handshake otherwise invalid');
                connection.write(Buffer.from(`HTTP/1.1 400 Bad Request

`));
                connection.destroy();
                return;
            }

            this.clients.push({connection: connection, buffer: []});
            const response = this.__formHeader(key);
            connection.write(response);
            // upgrade status of this connection -> change listener!
            this.onconnect();
            connection.on('data', data => {
                this.__handleMessage(data);
            })
        } else {
            // error-ish
            console.error('Unacceptable request recieved, handshake aborted.');
            connection.write(Buffer.from(`HTTP/1.1 400 Bad Request

`));
        
        }
    }

    __handleMessage = (data) => {
        // TODO parse client message properly!
        for (let client of this.clients) {
            const {connection, buffer} = client;
            const firstByte = data.readUInt8();
            const secondByte = data.readUInt8(1);
            const opcode = firstByte & (1+2+4+8);
            // OKAY note that MSB is first
            // do NOT misread a standard like that again
            if (firstByte & (64+32+16) || !(secondByte & 128)) {
                // against protocol 
                // at least one of RSV1, RSV2, RSV3 set
                // OR no mask indicated - client messages are always masked.
                this.onerror('Client sent a malformet packet');
                return;
                // remove...
            }

            switch(opcode) {
                case 0: // continuation frame, same code as text frame
                case 1: { // text frame, with handling of FIN
                    let message = this.__decodeMessage(data);
                    // check if FIN is set
                    if (firstByte & 128) {
                        // final package in message, good to go.
                        if (buffer.length) {
                            // there were more parts of the message!
                            message = [...buffer, message].join('')
                            client.buffer = [] // reset buffer
                        }
                        this.onmessage(message);
                    } else {
                        // then this is NOT the first part of the message.
                        buffer.push(message);
                    }
                    break;
                }
                case 8: {
                    // Comply with this close request
                    const message = this.__decodeMessage(data);
                    console.log('Received close request:', message);
                    // send our own close frame
                    const response = this.__encodeMessage('Closing as requested', true);
                    connection.write(response);
                    // destroy connection
                    connection.removeAllListeners();
                    connection.destroy();
                    break;
                }
                default:
                    this.onerror('Unsupported opcode');
                    break;
            }
        }
    }

    __decodeMessage = data => {
        let length, pre
        if (data[1] == 126) {
            // 2 bytes
            length = data.readUInt16(2);
            pre = 4
        } else if (data[1] == 127) {
            // 8 bytes
            length = data.readUInt64(2);
            pre = 10
        } else {
            // just 0-125 in data[1]
            length = data[1] & 127;
            pre = 2
        }
        // extract mask and masked data
        const mask = data.slice(pre, pre+4);
        const masked = data.slice(pre+4);
        let message = '';
        for (let i = 0; i < length; i++) {
            // every ith byte is masked by the (i%4)th byte of the mask 
            const unmasked = masked[i] ^ mask[i % 4];
            // parse char and form message
            message += String.fromCharCode(unmasked);
        }
        return message;
    }

    __encodeMessage = (message, close=false) => {
        let msgBuffer = Buffer.from(message);
        let length = msgBuffer.length;
        let lengthBytes
        if (length < 126) {
            lengthBytes = Buffer.allocUnsafe(1);
            lengthBytes.writeUInt8(length);
        } else if (length < Math.pow(2, 16)) {
            lengthBytes = Buffer.allocUnsafe(3);
            lengthBytes.writeUInt8(126);
            lengthBytes.writeUInt16BE(length, 1);
        } else {
            lengthBytes = Buffer.allocUnsafe(9);
            lengthBytes.writeUInt8(127);
            lengthBytes.writeBigInt64BE(length, 1);
        }
        // send opcode 8 if close=true
        // otherwise opcode 1 for txt msg
        return Buffer.from([(close? 0x88 : 0x81), ...lengthBytes, ...msgBuffer]);
    }

    listen = (port = 3001, callback) =>
        this.server.listen(port, callback || console.log('open'));

    /**
     * Send one message to all clients
     */
    broadcast = message => {
        console.log('Broadcasting:', message);
        const buffer = this.__encodeMessage(message);
        this.clients.forEach(({connection}) => connection.destroyed || connection.write(buffer));
    }

    // overridden by client, modelled after JS websocket interface
    onmessage = mesage => console.log(mesage);
    onerror = error => console.error('Connection error:', error);
    onconnect = () => console.log('Client connected');
    ondisconnect = () => console.log('Client disconnected');

}
