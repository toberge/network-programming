#!/usr/bin/env bash
STORE="exex"
PASSWORD="doofus"
set -m

echo "Lytt på loopback og filtrer 'tcp.port == 8000' i Wireshark"

keytool -genkey -alias "${RANDOM}Sign" -keystore $STORE

java -Djavax.net.ssl.keyStore="./$STORE" -Djavax.net.ssl.keyStorePassword=$PASSWORD JavaSSLServer &
bg
sleep 2
java -Djavax.net.ssl.trustStore="./$STORE" -Djavax.net.ssl.trustStorePassword=$PASSWORD JavaSSLClient

