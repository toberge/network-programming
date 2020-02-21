# Virtualisering


## virtualisering vs sandboxing

### Full virtualisering

la la la VirtualBox har de fleste brukt  
$\rightarrow$ dette er et helt system som virtualiseres

"CPU-maskiner" - 32-64 kjerner, fullverdig virtuelt OS i parallell

### Sandboxing

OS-nivå virtualisering (application sandboxing):  
Isolere ut enkelte applikasjoner uten å gjøre det i et _fullt_ system.  
Isoleringen skjer i kjernen (Linux foran).  
Nettlesere: En prosess for hver tab, disse kan være isolert via applikasjons-sandboxing.  
Docker kan være tungt, han nevner Flatpak som en mer lightweight virtualisering.

Han viser et utklipp fra Chrome-tegneserien of course.

`clone()` istedenfor `fork()` for OS-nivå virtualisering.

## `chroot`

Randomly lage en egen Linux-distro ved navn trondheim ¯\\\_(ツ)\_/¯  
Trenger noen binaries, lager mappe med kopi av noen greier (`ls` og `bash`) og bruker `ldd` for å kopiere biblioteker som trengs.

Den kopieringen tho... vi trenger å gjøre litt mer enn bare det der.

han brukte en eller annen kommando (`ldd`) for å se alle biblioteker, nevner `gtk` og `pango` osv.

`sudo chroot . /bin/bash` der /bin/bash er i den nye rota.

## Linux namespaces

`chroot` kun filsystem  
Namespaces isolerer på _alle_ nivå, gjennom `mnt` og `pid` for prosesser  
"8 kan bare se seg sjøl, kan ikke kommunisere med prosesser utenfor"  
Får en isolert prosesstruktur  
Kan isolere på flere måter _samtidig_

Kodeeksempel i [namespaces.cpp](namespaces.cpp)

## Docker

Bruker Linux namespaces og kan således gjenbruke vertens kjerne.  
\- denne Linux-kjernen kan kjøres som et gjeste-OS i andre operativsystemer.  

`docker run -ti archlinux /bin/bash` yeah yeah  
og poengterer at det gir et _veldig_ barebones system  
og at det går _veldig_ mye kjappere å starte enn å installere en ny VM

Køyre ting og så blir det rydda opp etterpå

`rm -rf /` og poengtere at bare noen spesielle mapper gjenstår

Typisk: Skrive `Dockerfile` så du ikke trenger gjøre alt oppsett manuelt...

Sammenlikner med git - men her har vi mer lagbasert struktur, ikke commits. Kommandoer i en Dockerfile blir cachet.

Install Docker yes yes

`sudo usermod -aG docker $USER` to add urself to docker group

Snakker om øvinga - den blir faktisk GØY

## Flatpak

Android bruker ikke namespaces for isolering --> ikke nødvendigvis like sikkert  
but guess what  
Flatpak bruker namespaces og gir begrensede, satte tilganger, kjører i alle distroer osv. (desktop-applikasjoner doe)

## Unikernels

Enda mer lettvekter enn Docker, hver applikasjon har en bitteliten kjerne med det som trengs til applikasjonen (library operating system)

IncludeOS - utviklet i Oslomet.  
Skriver at du skal ha tilgang til nettverket, får de driverne, og that's it, det er hele systemet, du slipper et helt OS-image, veldig veldig barebones

VirtualBox på minutter > Docker på sekunder > unikernel på millisekundnivå

## Wayland

> no skal eg setje opp noe som kjem te å sjå litt rahrt ut

wtf did he do to the Blackboard

Tilgang til alt mulig i display serveren != security  
En tenkt applikasjon som leser skjermbildet og plasserer sitt eget innloggingsvindu over Blackboard  
$\Rightarrow$ trenger et sikkert vindussystem

Wayland er det nye kule, lagd med tanke på sikkerhet...  
Applikasjoner skal ikke kunne lese av skjermen, ta screenshots, manuelt posisjonere eget vindu osv.  
Ikke mulig å injisere input-events heller.
