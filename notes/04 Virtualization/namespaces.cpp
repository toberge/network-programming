#include <sched.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <iostream>

using namespace std;
// yeah

int main() {
    char child_stack[1024*1024];

    pid_t child = clone([](void *) {
        // nytt namespace så vi er nummer 1!
        cout << "hey I am a kid and I am " << getpid() << endl;
        // skal ikke ha tilgang til andre prosesser i systemet
        return 0;
    }, child_stack + 1024*1024, CLONE_NEWPID, nullptr);

    // skal få annet resultat
    cout << "parent says " << child << endl;

    waitpid(child, nullptr, 0);
}
