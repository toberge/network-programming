# 07 WebSocket

A simple WebSocket library with a drawing board server.

## The task

Ta utgangspunkt i rfc6455 og programmer et WebSocket-server bibliotek som gjennom socket-programmering (du skal ikke bruke et ferdig WebSocket bibliotek på serversiden):

- [x] Utfører handshake med klienter
- [x] Leserer korte meldinger fra klienter
- [x] Kan sende korte meldinger til tilknyttede klienter
- [x] Frivillig: lag et tegnebrett som flere klienter kan tegne på samtidig.

## The result

WebSocket library in `whereissock.js`,  
drawing board server in `server.js`.

![demo](it_works.png)

## Usage

Run `npm install` and `npm start` to start the server.

Open [localhost:3000](http://localhost:3000)
or `<your ip on the local network>:3000`
in your browser.
