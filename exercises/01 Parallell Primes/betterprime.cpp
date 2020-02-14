#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <algorithm>
#include <math.h>

using namespace std;

/**
 * betterprime.cpp
 * Go look in eratosthenes.cpp, that one is way better
 * This one uses the very naive "omg are you divisible by me?" method
 */

// Naive prime check
// If run sequentially it would have 
// a time complexity like O(sqrt(n)!)
// ...which is rather bad
bool isPrime(int number) {
    // at least it *now* only runs up 'til the sqrt of the number
    for (int i = 2; i < floor(sqrt(number)); i++) {
        if (number % i == 0) {
            return false;
        }
    }
    return true;
}

int main(int argc, char const *argv[]) {
    int start = 0;
    int end = 1000;
    int numThreads = 10;
    if (argc == 3) {
      start = stoi(argv[0]);
      end = stoi(argv[1]);
      numThreads = stoi(argv[2]);
    }
    
    vector<int> primes;
    mutex primeLock;
    vector<thread> threads;
    vector<int> work(numThreads);
    
    if (start < 2) start = 2;
    // int increment = (end - start) / numThreads;
    // DO NOT jump by total numbers to examine divided by number of threads
    
    for (int i = 0; i < numThreads; i++) {
        threads.emplace_back([&primes, &primeLock, start, end, &work, i, numThreads] {
            // loop from start to end, jump by number of threads
            for (int j = start + i; j <= end; j += numThreads) {
              // do actual work of checking if prime
              if (isPrime(j)) {
                  // if it is, lock and add to list
                  lock_guard<mutex> lock(primeLock);
                  primes.push_back(j);
              }
              work[i]++;
            }
        });
    }

    for (auto &thread : threads) {
        thread.join();
    }
    
    sort(primes.begin(), primes.end());
    for (auto prime : primes) {
        cout << prime << ", ";
    }
    cout << endl << "Threads:" << endl;
    
    int total = 0;
    for (int i = 0; i < numThreads; i++) {
        cout << "Thread " << i << ": " << work[i] << endl;
        total += work[i];
    }
    cout << "Average work " << total / numThreads << endl;
    
    return 0;
}
