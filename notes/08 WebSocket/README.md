# WebSocket

## om denne tingen

+ Raskere enn å gjøre HTTP-kall
+ Det beste: toveis-kommunikasjon!
    + så server kan også ta initiativ og sende til klienten

Standard: rfc6455  
Ikke les A-B men bruk den som oppslagsverk  
Heldigvis er den en av de som er _bedre_ skrevet.

Viktigste kapitler:
+ 4. Opening Handshake
+ 5. Data Framing

## et lite eksempel

For å sette opp en WebSocket må spesifikk HTTP-forespørsel gjøres:
+ Upgrade: websocket --> hva vi skal oppgradere til
+ Connection: Upgrade --> ikke Keep-Alive (føles litt unødvendig, aye)
+ Sec-WebSocket-Key: massebokstaverogtall (i base64) --> for å sikre at serveren ikke bare er en proxy-server som har cacha en respons
+ Sec-WebSocket-Version: 13

Siden websocket er nyere protokoll og det var en del proxy-servere som kun støtta HTTP, måtte det gjøres noen tiltak for at websocket ikke skulle forstyrre dem for mye

base64: heksadesimal x 4, flere sifre y'know

Serveren skal så svare med
+ Sec-WebSocket-Accept: massebokstaverogtalligjen --> bevise at den støtter websocket 

## handshake

Fra klienten får vi en key, denne må vi prosessere slik:
1. SHA1 av key + en konstant som er i websocket-standarden
2. base64 av den hashen (NB hashen blir bytes).

le teacher sliter med å sha1-e via random nettsider (så vanskelig dette skal være)

kobler seg opp, yo (huska ikke syntaks as usual)

tegnebrett hype

## yes

le ferdig websocket-bibliotek er jo greit men vi skal lage vårt eget!

error og stuff

## meldinger

fra klient: må gjøre stuff med den  
fra server: kan se det klart i Wireshark, men to innledende bytes er rare

Begge starter med byte 81  
neste er lengden på meldinga, eks. er 0b 12 tegn   
(med http må du lete etter `\r\n`)

okei:
+ Første byte angir type melding, 81 for tekst
+ Andre byte angir lengden - meldinger < 126 bytes trenger kun 7 bits for lengden, da brukes den mest signifikante til å fortelle om meldingen er maskert eller ei.
    + men serveren skal _aldri_ maskere meldinger...  
      det er her det begynner å bli litt rart, ja.
    + masken gis i de neste 4 bytene (i en handshake?)

Vi vil at klientenes meldinger skal se ganske forskjellige ut - og _igjen_ er det på grunn av disse proxy-serverne som ellers hadde sendt cachede svar.

symmetrisk kryptering: like lang nøkkelstreng som meldinga er lang, hvis lang melding så bare repeterer du den nøkkelstrengen

NB: xor-e med tall i nøkkelstreng  
eks b4 b5 03 2a nøkkelstreng, da skal første hex xor-es med b4  
(det er det vi skal gjøre, for å få ut det klienten skrev)

```
for hver 4n + x byte:
    denne byte XOR (x+1)-te byte i nøkkelstrengen
```

det klienten gjør: hex(ord('char') ^ nøkkel-byte)

gotta use knowledge of sockets, indeed.
