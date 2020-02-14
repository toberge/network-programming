# Asynkrone greier

Måter å gi oppdrag til spesielle tråder - worker threads

I fjor unngikk vi å se problemene selv, med asynkron programmering i JS

CONDITION VARIABLES YAAAAAY

Bruker stort sett ikke disse på høyere nivå, men hvis vi koder biblioteker selv, derimot...

Øving 2: Språket MÅ ha CVer, kan ikke være JS eller Python

## Mysteriet condition variables

Vi vil vente på noe.

Første problem: while-loopen kjører hele tida uten å sette seg på vent
+ Vi vil vente på en fornuftig måte.
+ Gir prosessoren litt mindre å gjøre ved å legge til en liten venting
+ Bør også sørge for at operasjonen er atomisk -> lås eller atomic
    + Som regel går det greit, men vi bør være på den sikre sida her
+ Uansett hva du setter ventetida til blir det
    + for mye arbeid (sjekker hvert n-te millisekund)
    + for lang tid før tråden får fortsatt
+ (yield er noe annet)
+ Introduserer nå en mutex (lås) og en condition variable
    + ikke lenger nødvendig med atomic ofc
    + kan ikke lenger bruke `lock_guard`, må bruke `unique_lock` isteden
    + Vi skal nå "vente på" condition variable-en
    + `cv.wait(lock)` der lock er den `unique_lock` vi satte
        + Låser opp mutexen og venter på en notfiy
        + Når den vekkes opp av en notify, låser den mutexen igjen
    + Vi vekker opp alle eller én som venter på CV-en med `notify_all()` eller `notify_one()`
        + Broadcast vs signal
        + Virker som vi heldigvis bruker ~~Hoare~~ **Mesa** semantics
    + deadlock pga at mutexen var låst mens vi kalte `notify_one()`
        + må passe på å låse opp mutexen før vi notifier

> I C++ og Rust er det bare å pøse på med abstraksjoner for å gjøre koden litt mer leselig

Andre problem: øhhhhhhh


## Funksjonsobjekt

Liste med funksjoner – hvordan?  
Vanligere å lagre funksjoner i liste enn vector, siden lister er lettere å modifisere (og vi kanskje vil fjerne oppgaver vi er ferdige med)

```C++
#include <functional>
#include <list>
#include <iostream>

using namespace std;

void func() {
    cout << "func()" << endl;
}

int main() {
    // pga lettere å modifisere
    // bruker vi lenka/dobbeltlenka liste.
    list<function<void()>> tasks;
    // NB: void() som signatur

    tasks.emplace_back(func);
    tasks.emplace_back([]() -> void {
        cout << "Hello from lambda calculus" << endl;
    });

    // Vi vil ta bort oppgavene fra denne lista - how?
    for (auto it = tasks.begin(); it != tasks.begin(); /*it++*/) {
        auto &task = *it; // dereferer objektet iteratoren peker på
        task();
        // fjerner element og returnerer iteratoren til neste element
        it = tasks.erase(it);
        // ikke it++, vi vil faktisk fjerne!
    }
}
```


# Worker threads!

Henter oppgaver som skal utføres, og utfører dem.  
Altså oppgaver som kan kjøres i parallell  
Samme som "thread pool"

Vente (på å få arbeid, på at arbeid blir klart til å gjøres)  
$\Rightarrow$ bruker condition variables

+ Lager liste
+ Legger til oppgaver
+ Kjør worker threads
    - vist enkelt
    - popper bare task ut fra lista
    - viser kaotisk utskrift of course
    - av og til krasjer det...
    - $\Rightarrow$ bedre med en CV

Progresjon:
+ yeah, han legger inn en mutex først
+ og så må vi fikse at de kjører sekvensielt nå, siden han la låsen rundt hele henting- og utføringsrutinen

Yeah, vi må vente på å få ei oppgave istedenfor å konstant sjekke om vi har ei oppgave å gjøre (kjører altså veldig mange ganger i sekundet - avhengig av klokkefrekvens og hvor mange operasjoner som inngår)


## Event loop

En event loop inneholder worker threads?  
Delegerer "blocking tasks" til worker threads, gjør en del ting selv

F.eks. "hei, lese en fil" => "ok" => "ferdig"

Snakker om håndtering av state delt mellom worker threads -> pass på å bruke mutex  
Event loop skal forenkle dette? (håndtering i én tråd)

Event loops kommer fra GUI-verdenen (håndtere events av div. slag)
Oppdatering av farger i event loop vs. analyse av kode i worker thread

Altså: worker threads for mer tidkrevende oppgaver, event loop for små oppgaver som jobber mot shared state


## Øvinga

Lage klasse for worker threads, skal kunne starte, legge til og stoppe.  
En event loop blir en entråds Workers-klasse

Lage en ting med set_timeout for å postpone til etter et gitt antall millisekunder – der har vi den frivillige delen av oppgava, å forbedre ved å bruke epoll (Linux-spesifikt) (evt. bare opprette en tråd for ventinga...)

*epoll*

Socket-oppkobling blir også håndtert som ei fil - når data er tilgjengelig der er de sendt fra ei anna datamaskin

Workers-implementasjoner kan gjenbrukes... Vår vil være såpass enkel at vi kan bruke den til småtteri der vi ikke trenger alt mulig annet.

Problemet med funksjoner som det her (epoll): Såpass kompleks at de ikke er standardiserte (kun epoll på linux, annen greie på win, mac, etc.) - en platformuavhengig variant vil måtte bruke det som fins på OS-et den skal kjøres på, aka flere alternativer...
