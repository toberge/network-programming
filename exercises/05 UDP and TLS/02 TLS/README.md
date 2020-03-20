# TLS stuff

using an SSL client-server example by some dude  
run `ssl.sh` and write some stuff to see it in Wireshark

## Proof of encryption:

### With key exchange

![packets](packets.png)  
![fields](data_in_fields.png)  
![follow](data_on_follow.png)

Looks pretty darn unreadable to me.

### Once again

\- with no need for lengthy encryption handshake

In terminal:

```
Enter something:
HELLO HELLO
Server log: HELLO HELLO
Client got back: HELLO HELLO
```

In Wireshark:

![fewer packets](fewer_packets.png)

Pretty clear that the transmitted data is encrypted.
