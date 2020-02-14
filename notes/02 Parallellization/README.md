# Atomic typer
what is this?  
I dare say it sounds familiar...

```c++
#include atomic
```

# Parallellisering CPU vs GPU
GPUer har hundrevis av kjerner  
f.eks. matrisemultiplikasjon (mange kjerner + operasjon som er lett å splitte opp = sant)

manuell opprettelse av tråder --> mye hjernekapasitet kreves for å sette seg inn i det

med flagg -fopenmp får vi en suboptimal parallellisering og setter begrensninger på koden
# pragma omp whatever, diverse stuff du skriver før parallelliserbar operasjon


**Funksjonelle algoritmer kan parallelliseres!**

vi er i framtida der vi kan bruke funk alg og få til automatisk parallellisering

transform: utføre på a, lese av a, lese av b, ouput i c.

stuff stuff stuff...

Det går også an å bruke integrerte GPUer til slike operasjoner, kan skje automatisk i noen CPUer om ikke så altfor lenge?

OpenCL is a total mess to look at

to store maskinlæringsbibliotek: tensorflow fra Google, torch fra Facebook

data må kopieres over til grafikkortet  
den første a-en er oppretta i CPUens minne  

# Prosesser!
kræsj i en påvirker ikke en annen (og de har separate minneområder)  
men vi har mer overhead med prosesser  
og kommunikasjon er verre

look at mah shell, it spawns processes all the time

## Kommunikasjon
+ statuskode ved exit (oh, I know)
+ stdout

vi kan få stdout fra annen prosess:
```c++
using namespace std;
using namespace TinyProcessLib;

int main() {

int a = 42;

// delt minne? neida!
Process process([&a] {
    a++;
    cout << a << endl;
    cout << "from child" << endl;
    exit(0);
}, [](const char *data, size_t n) {
    // data fra barneprosessen!
    cout << string(data, n) << endl;
});

process.get_exit_status();
// after execution of process
cout << a << endl;

}
```
unix: fork then exec (omg u didn't remember)

går bare på unix-systemer, på windows trenger vi eksterne .exe-filer (wtf why)

```c++
// f.eks. i en Process
execl("/bin/echo", "/bin/echo", "Hello", "warld", nullptr);
```

```c++
// annet eksempel med piping til cat
Process process("cat", "",  [](const char *data, size_t n) {
    cout << string(data, n) << endl; // output: Hello whatever
}, nullptr, true);

process.write("Hello wrwijgwrg\n");
process.close_stdin();
process.get_exit_status();
```

