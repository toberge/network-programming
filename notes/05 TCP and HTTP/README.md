# TCP og HTTP

No Eidheim this time, how sad  
What, bare tre kvarter med mister Holt?

Neste år er vi tredjeklassinger og må være mer sjølstendige -> hint til høyere grad av sjøllæring...  
Det er veien det går & det funker bra, sier han

## Distribuert system

Eksempel: DNS, fordelt på mange maskiner rundt om i verden

Typisk klient-tjener-løsninger, en tjener venter på forespørsler og klienter sender forespørsler

Hva mener vi egentlig? Applikasjoner, ikke maskinene - vi sier tjener- og klient-maskin når vi snakker om hardwaren.

Hva hvis vi snur rollene? Det er fullt mulig.

Programmering direkte mot TCP-laget - lavnivå.  
Hvorfor ikke bruke web til alt? Det har sine begrensninger.
+ ligger på høyere nivå -> blir tregere
+ må ned i stacken for å få ting raskere (f.eks. kan spill foretrekke å bruke UDP)

REST vet vi  
RMI vet vi ikke?  
Kutter ut SOAP (og RMI). Er ikke aktuelt lenger  
Hopper over mellomvare også

## Sockets

Porter opp til sockets

Eksempelkode i Java oh gee

C++: når vi kommuniserer på dette nivået kan vi få problemer med datatypene (i Java vil klient og tjener begge kjøre på virtuell maskin og datatypene vil være de samme) -> en int kan være av forskjellig lengde osv. (Rust FTW!)

Socket-APi laget for å hindre at vi må gå altfor langt ned i stacken, trenger ikke gå ekstremt langt ned.

En socket er ei IP-adresse og et portnummer.

Sockets ligger til grunn for alt av higher-level apllikasjoner  
nevner `telnet` - tjener du kan koble deg opp til og sende kommandoer til.

## Opp- og nedkobling

En del datapakker går mellom klient og tjener

handshake: SYN A -> B, SYN+ACK B -> A, ACK A -> B  
(B er tjener, A er klient)

Etter handshake går flere pakker mellom klient og tjener inntil tilkoblinga avsluttes

## Oppkobling med sockets

(dette blir lettere)

Lager en socket-port og lar socket-tjeneren lytte på den  
Får beskjed om at klient prøver å koble seg opp

Klient sender oppkoblingsforespørsel, tjener avviser eller aksepterer, frigjør porten og setter opp en ny for å kommunisere med klienten så andre klienter kan kommunisere med samme port på tjeneren når de sender første forespørsel.

## Eksempelkode (jAvA)

`import java.net.*;`

bla bla lage ny ServerSocket, kalle accept() for å blokkere til vi får en forespørsel og vi får et Socket-objekt  
Klienten lager en ny socket på nettadresse og port  
og så kommuniserer vi via strømmer...  
Lukke ved avslutning naturligvis osv.

Gr gjennom eksempelkode... manuell lukking av strømmer, ingen try-with-resources (skal være nok å lukke socket, hm)

he using konsole in addition to gnome terminal? oh my what

For å få nettverksforbindelse må du bruke faktiske IP-adressa til maskina (di)  
Ikke sitt og dall på ei maskin  
Og han bruker telnet istedenfor sin klient som et eksempel  
`telnet localhost 1250`

NB: telnet krever blank linje etter forespørselen din før du får respons  
`GET /thing.html HTTP/1.0` og litt mere stuff, gjerne, derfor tom line for å terminere

HTTP er et veldig enkelt lag over socket-laget  
TYpisk gjør nettleser forespørsel heller, yeah...

## Øvinga

Vi skal lage en webtjener, det er ikke så _veldig_ vanskelig  
Ta imot socket-forespørsel og send tekst tilbake

Vi skal også lage en kalkulator...

Hva slags format bør utvekslinga være på?
+ sende regnestykket som "4 + 3" og svar som "4 pluss 3 = 7"
+ eller bare 7 tilbake
+ etc.

Dette **må** avgjøres!  
Vi må tenke på formatet vårt sjøl.  
En av fordelene og ulempene til socket-programmering...

Java-RMI hadde gjort at man hadde sluppet dette, samme med SOAP.  
Poenget med RMI: Vanlige Java-objekter på klient- og server-sida.

Må lage en metode som skal gjøre utregninga...

## kjekke programmer

Det har blitt mer fokus på sikkerhet nå for tida  
Alternativ til telnet: openssl (et bibliotek _og_ et program) - kan lage sertifikater osv.

## TLS og ASIO

TLS/SSL: litt info om hvordan... Java-implementasjon kan bli veldig lik som for ukrypterte forbindelser. Trenger sertifikater

Asynkron IO/socket (asio) yay yay  
+ mulighet for non-blocking IO (trenger ikke stå og vente hele tida)
+ andre ting kan skje mens man venter på ~~Godot~~ IO
+ synkorn IO 

noen spørsmål? nej då

Pleier ikke være noen problemer med den øvinga her  
(greit å vite, but really tho?)

Litt forskjell på når det er øvingsinnlevering AHEM OKEI
