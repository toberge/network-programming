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