#ifndef TIMER_H
#define TIMER_H

#include <functional>
#include <thread>
#include <vector>
#include <list>
#include <sys/epoll.h>
#include <sys/timerfd.h>
#include <atomic>
#include <map>

using namespace std;

class Timer {
    private:
    const int epoll_fd;
    atomic<int> stop_fd;
    std::thread timer_thread;
    // Events get loaded into this vector
    vector<epoll_event> events;
    // Hashmap translating event fd to task
    map<int, function<void()>> timeouts;
    int create_event(int milliseconds);

    public:
    Timer();
    void set_timer(std::function<void()> task, int milliseconds);
    void start();
    void stop();
};

#endif //TIMER_H
