#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <atomic>

using namespace std;

// Naive prime check
bool isPrime(int number) {
    for (int i = 2; i < number; i++) {
        if (number % i == 0) {
            return false;
        }
    }
    return true;
}

int main(int argc, char const *argv[])
{
    vector<int> primes;
    mutex primeLock;

    atomic<int> number(2);

    const int limit = 10000;
    int numThreads = 10;
    vector<thread> threads;
    vector<int> work(numThreads);
    
    for (int i = 0; i < numThreads; i++) {
        threads.emplace_back([&primes, &primeLock, &number, &work, i] {

            while (number < limit) {
                {
                    // increment examined number
                    number.fetch_add(1);
                }

                // do actual work of checking if prime
                if (isPrime(number)) {
                    // if it is, lock and add to list
                    lock_guard<mutex> lock(primeLock);
                    primes.push_back(number);
                }
                work[i]++;
            }
        });
    }

    for (auto &thread : threads) {
        thread.join();
    }

    // for (auto prime : primes) {
    //     cout << prime << ", ";
    // }
    cout << endl << "Threads:" << endl;
    
    int total = 0;
    for (int i = 0; i < numThreads; i++) {
        cout << "Thread " << i << ": " << work[i] << endl;
        total += work[i];
    }
    cout << "Average work " << total / numThreads << endl;
    
    return 0;
}
