#include <algorithm>
#include <execution>
#include <iostream>
#include <thread>
#include <vector>

using namespace std;

// dei fleste hatar det, for det er styr å jobbe med (integrert) grafikkort

int main() {
  vector<int> a = {0, 2, 4};
  vector<int> b = {0, 3, 4};
  vector<int> c(a.size());

  transform(execution::par, a.begin(), a.begin(), b.begin(), c.begin(), [](int a, int b) {
    cout << this_thread::get_id() << endl;
    this_thread::sleep_for(1s);
    return a + b;
  });
}