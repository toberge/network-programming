#include <atomic>
#include <iostream>

using namespace std;

enum class State { crouching,
                   sitting,
                   rising,
                   standing,
                   running };

int main() {
  // Kan bare bli brukt på veeeldig basic verdier
  // standardiseringen av dette påvirket prosessorarkitektur
  atomic<State> state(State::crouching);

  auto expected = State::crouching;
  auto expectations = State::rising;
  // atomic change in enum if it is of expected value
  // dette er det vanskeligste vi noen gang kommer til å bruke
  if (state.compare_exchange_strong(expected, State::rising)) {
    cout << "stuffing up" << endl;
  } else if (state.compare_exchange_strong(expectations, State::standing)) {
    cout << "standing." << endl;
  }
}
