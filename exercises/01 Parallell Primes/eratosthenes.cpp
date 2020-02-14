#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <math.h>

using namespace std;

/**
 * eratosthenes.cpp
 * Celebrating one month of spelling Eratosthenes the wrong way
 * 
 * Finds primes in a parallell operation with a specified number of threads,
 * using the Sieve of Eratosthenes algorithm.
 */

// Single marking run of eratosthenes sieve
void eratosthenes(vector<bool> &isPrime, mutex &markerLock, int i, int limit) {
    bool yes;
    markerLock.lock();
    yes = isPrime[i-1];
    markerLock.unlock();
    if (yes) {
        int multiple = i*i - 1;
        while (multiple < limit) {
            markerLock.lock();
            isPrime[multiple] = false;
            markerLock.unlock();
            multiple += i;
        }
    }
}

int main(int argc, char const *argv[]) {
    int start = 10;
    int end = 200;
    int numThreads = 10;
    if (argc == 3) {
      start = stoi(argv[0]);
      end = stoi(argv[1]);
      numThreads = stoi(argv[2]);
    } else {
        cout << "Usage: <start> <end> <number of threads>" << endl;
    }

    vector<thread> threads;
    vector<bool> isPrime(end);
    mutex markerLock;
    for (int i = 1; i < end - start; i++) {
        isPrime[i] = true;
    }

    // only marking multiples of up to the square root of the limit
    // going higher is NOT necessary, since any square of those numbers
    // would be equal to or higher than the limit
    int stop = floor(sqrt(end));

    for (int t = 0; t < numThreads; t++) {
        threads.emplace_back([start, end, stop, t, numThreads, &isPrime, &markerLock] {
            if (t == 0) {
                // special case for 2
                eratosthenes(isPrime, markerLock, 2, end);
            }

            for (int i = 3+t; i < stop; i += 2*numThreads) {
                eratosthenes(isPrime, markerLock, i, end);
            }
        });
    }

    for (auto &thread : threads) {
        thread.join();
    }

    for (int i = start - 1; i < end; i++) {
        if (isPrime[i]) {
            cout << (i+1) << endl;
        }
    }
    
} 