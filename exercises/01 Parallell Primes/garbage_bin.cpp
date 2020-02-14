#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <math.h>

using namespace std;

void erasthothenes(int start, int end, int square, std::vector<bool> &markers) {
    for (int i = 0; i < end - start; i++) {
        markers[i] = false;
    }
    
    for (int i = 2; i < square; i++) {
        if (markers[i]) { // if marked as prime

            // determine where to start
            int index;
            if (start % i > 0) {
                index = (i*i - 1 + i * (start / i) + i) % start;
            } else {
                index = 0;
            }
            
            // mark all multiples of i
            while (index < end) {
                markers[index] = false;
                index += i;
            }
        }
    }
}


int main(int argc, char const *argv[])
{
    vector<thread> threads;
    vector<vector<bool>> markerHolder;
    int numThreads = 10;
    int n = 100;
    int square = floor(sqrt(n));
    int increment = n / numThreads;
    
    for (int i = 0; i < n; i += n / numThreads) {
        markerHolder.push_back(vector<bool>(n / numThreads));
        vector<bool> markers = markerHolder[i / increment];
        threads.emplace_back([i, increment, square, &markers] {
            //erasthothenes(i, i + n / numThreads, floor(sqrt(n)), &markers);
            int start = i;
            int end = start + increment;
            
            for (int i = 0; i < end - start; i++) {
                markers[i] = true;
            }
            
            for (int i = 2; i < square; i++) {
                if (markers[i]) { // if marked as prime
            
                    // determine where to start
                    int index;
                    if (start % i > 0) {
                        index = (i*i - 1 + i * ((start - i*i - 1) / i) + i) - start;
                    } else {
                        index = 0;
                    }
                    cout << "from " << index << " with " << i << ", " << start << endl;
                    
                    // mark all multiples of i
                    while (index < end) {
                        markers[index] = false;
                        index += i;
                    }
                }
            }
        });
    }
    
    for (auto &thread : threads) {
        thread.join();
    }
    
    for (int i = 0; i < numThreads; i++) {
        for (int j = i * increment; j < i * increment + increment; j++) {
            if (markerHolder[i][j]) {
                cout << j + 1 << endl;
            }

        }
    }

    return 0;
}
