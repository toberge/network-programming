#include <iostream>
#include <thread>
#include <mutex>

using namespace std;

int main(int argc, char const *argv[])
{
    int sum = 0;
    mutex sumLock;

    thread first([&sum, &sumLock] {
        for (int i = 0; i < 1000; i++)
        {
            sumLock.lock();
            sum++;
            sumLock.unlock();
        }
    });

    thread second([&sum, &sumLock] {
        for (int i = 0; i < 1000; i++)
        {
            // new thing, at creation it locks
            // unlocks when it is destroyed (going out of scope)
            lock_guard<mutex> lock(sumLock);
            //sumLock.lock();
            sum += 10;
            //sumLock.unlock();
        }
    });

    first.join();
    second.join();

    std::cout << sum << endl;

    return 0;
}
