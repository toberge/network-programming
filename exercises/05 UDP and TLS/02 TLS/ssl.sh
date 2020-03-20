#!/usr/bin/env bash
STORE="keystore"
PASSWORD="doofus"
set -m

echo "Lytt på loopback og filtrer 'tcp.port == 8000' i Wireshark"

javac *.java

keytool -genkey -alias "${RANDOM}Sign" -keystore $STORE > >(sed 's/^/server: /')

java -Djavax.net.ssl.keyStore="./$STORE" -Djavax.net.ssl.keyStorePassword=$PASSWORD JavaSSLServer &
bg
sleep 2
echo "Snart kan du skrive noe og se det repetert to ganger (av server og klient)"
java -Djavax.net.ssl.trustStore="./$STORE" -Djavax.net.ssl.trustStorePassword=$PASSWORD JavaSSLClient

