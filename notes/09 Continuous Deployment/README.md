# Continuous Deployment


Erstatte koden som kjører på en server (i produksjon)

## logge på

Finner ip på serveren med `ip address show`

Lager ny bruker med `sudo useradd --create-home app` (uten passord eller rettigheter)  
Denne skal kjøre applikasjonen.  
Lite rettigheter -> mindre en cracker kan gjøre med den

Skifte til brukeren med `sudo su app`

## Målet nå:
1. En klient skal kunne knytte seg opp mot serveren
2. Skal kunne kopiere filer over til serveren

## SSH-stuff
authenticity can't be established blah blah do you trust this fingerprint?

To ting skjer her nå:
+ **Oppretter en kryptert forbindelse** - symmetrisk kryptering
+ men de har ingen fellesnøkkel!
+ så serveren må ha et nøkkelpar
+ klienten oppretter en symmetrisk nøkkel
+ som krypteres med serverens public key
    + noen kunne ha latt som om de var serveren med denne IP
    + kunne fått public key fra en hacker
    + som kunne tatt info fra deg
    + sikreste måte: "DER, der står serveren"
        1. fysisk tilgang -> you know it be dis server
        1. kjør `ssh-keyscan <denne serverens ip>`
        1. legg inn i `~/.ssh/known_hosts` på klient
        1. kan da `ssh`-e inn fra klient
        1. NB: passordløs pålogging
+ lager så klientens nøkkelpar med `ssh-keygen -t rsa`
    + ligger slik:
       `~/.ssh/`
       `|- id_rsa` privat nøkkel (nesten så han `cat`-a den)
       `|- id_rsa.pub` offentlig nøkkel - gib 2 server
       `|- known_hosts` fra tidligere
+ legger det inn i serverens `.ssh/authorized_keys`
    + nevner Windows sine linjeskift  
      > "resten av verda bruker jo selvfølgelig bære `\n`"  
      > pass på at dokker ikkje lime inn carriage return
+ serveren vil så sende en liten "challenge" til klienten når den prøver å logge på
    + serveren lager tilfeldig tall, krypterer det med public key til klient
    + klienten er da den eneste som kan dekryptere dette tallet

## koble gitlab til serveren!

må legge til
+ privat nøkkel `id_rsa` så gitlab kan identifisere seg
+ `known_hosts` så gitlba vet at den snakker med rett server

time to `git clone` and `npm install` and `npm start`

but what about changing the code and having it propagate?  
hm hm hmu

he making changes to asset file `index.html`  
he want it to update w/o server restarting  
he show the `gitlab-ci.yml`

## that CI file

Setter visse **hemmelige** variabler i GitLab-repoet  
Settings > CI > Variables  
Disse kan brukes i CI-pipelinen

**NB**: huk av for Protect variable iallfall på private key  
da vil den kun brukes på protected branches  
og derfor `only: master` i skriptet

`echo $SSH_KNOWN_HOSTS" > ~/.ssh/known_hosts`  
for å kjenne igjen server

`chmod 600 ~/.ssh/id_rsa`  
fordi ssh krever spesifikk permission (rw)  
`echo $SSH_PRIVATE_KEY" > ~/.ssh/id_rsa`  
for å kopiere den over

> husker ikkje ka default-settinga va her (prot branch tilgang)

og han viser innstillinger for protected branches, aye

## copy it over

`scp public/index.html app@<ip til server>:app/public`

kopiering over ssh yes

> etterkvart så ska vi nå restart applikasjonen også

egentlig bør vi bruke rsync for å _ikke_ kopiere filer som er like!  
`rsync --archive --delete --exclude='.git' --exclude='node_modules' . app@<ip til server>:app/`
+ `--archive` effektiviserer ved å skippe identiske filer
+ `.` for å bare ta heile sulamitten
+ `--delete` for å slette filer som er sletta i repoet
+ `--exclude='diverse'` for å ekskludere f.eks. git sine filer
+ mulig rsync må installeres...

passe på å være i rett matte når vi `npm install`-er  
og pass på å ta vare på `node_modules`:
```yaml
script:
  - npm install
artifacts:
  paths:
    - node_modules
```

**fortsettelse følger**

on my own: arch/manjaro krevde litt mer setup  
evt endre /etc/ssh/ssh_config  
starte sshd med `systemctl start sshd`  
og så er du good

## Restarte applikasjonen

- hva hvis vi endrer port eller noe?

Vi trenger et prosessadministrasjonsprogram  
in comes `systemd`

> det e jo en del diskusjona rundt det systemet...

La app-brukeren kjøre prosesser selv om den ikke er logga på:  
`loginctl enable-linger app`

(mulig du må opprette fila selv tho)

Må så lage `~/.config/systemd/user`-mappe

inn i fila `app.service`:
```ini
[Unit]
Description=App
; starte etter at nettverket er starta
After=network.target 

[Service]
; riktig hjemmekatalog (NB check this)
WorkingDirectory=/home/app/app
; hva vi skal starte
ExecStart=npm start
Restart=always
; "litt usikker på hva dette betyr"
Type=simple
; miljøvariabler!
Environment=NODE_ENV=production

[Install]
; skal starte når maskina rebooter
WantedBy=default.target
```

Det første vi må gjøre er å si at vi skal scanne denne katalogen!  
`systemctl --user daemon-reload`  
og  
`systemctl --user enable app`

`--user` for at dette ikke skal ha admintilgang

Så kan `systemctl --user start app` starte servicen

Kan se status og evt. feilmeldinger med `systemctl --user status app`

kan også stoppe og restarte applikasjonen!

`systemctl --user restart app`

Fra gitlab CI: `ssh app@$SERVER_IP systemctl --user restart app`

Hvis vi vil kjøre på port 80 eller en annen port < 1024 kan vi ikke være vanlig bruker  
Kan bruke `iptables` til å omdirigere trafikken til 3000 eller noe

blah blah diverse firmaer lar deg bruke en Docker-container i en Kubernetes-cluster blah blah

> fort litt partisk kanskje

Yeeeeah, jeg kan continuous integration nå ja...

