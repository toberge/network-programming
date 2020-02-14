#include <atomic>
#include <iostream>
#include <thread>

using namespace std;

// rust: Atomically reference counter std::sync::Arc
// c++: std::shared_ptr

// kan implementeres med atomic<int>
// en atomic<int> har .fetch_sub(int) som gir gammel verdi i tillegg til å subtrahere,
// i en atomisk operasjon

// huske at atomic fins og at de kan kjøres på spseielle måter som gjør at det er sikkert å bruke dem i flere tråder

int main() {
  // Pointere blir ikke destruert når du går ut av skopet, new string() gir en pointer
  // str er bare addressa til strengen i minnet
  // we must delete the pointer manually
  // men det fører ikke til at innholdet i minneområdet fysisk slettes,
  // det bare frigjøres til senere!
  auto str = new string("Hi there you foolish being");

  thread t;

  {
    // en sikrere greie:
    shared_ptr<int> ref_counted(new int(231));

    t = thread([str, ref_counted] {
      // asterisk to get value at address
      cout << "From the thread: " << *str << endl;

      // to sleep in thread:
      this_thread::sleep_for(1s);
      cout << "value from thread: " << *ref_counted << endl;
      cout << "count from thread: " << ref_counted.use_count() << endl;
    });

    cout << *str << endl;
    cout << "value from outside: " << *ref_counted << endl;
    cout << "count from outside: " << ref_counted.use_count() << endl;
  }
  // nå skal ref_counted være slettet
  // siden antall referansetelleren er 0 og vi går ut av skopet

  // sletting av pointer != sletting av innhold
  // å slette på dette tidspunktet gir en feilmelding med -sanitize=thread
  //delete str;
  // med garbage collection vil den ha en visshet om at minneområdet ikke er i bruk før det slettes

  t.join();
  delete str;
}