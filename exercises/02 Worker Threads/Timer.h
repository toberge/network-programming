#ifndef TIMER_H
#define TIMER_H

#include <functional>
#include <thread>
#include <vector>
#include <sys/epoll.h>
#include <sys/timerfd.h>

using namespace std;

typedef struct st_event {
    function<void()> task;
    int fd;
} timeout_event;

class Timer {
    private:
    const int epoll_fd;
    int stop_fd;
    std::thread timer_thread;
    vector<epoll_event> events;
    vector<timeout_event> timeouts;
    int create_event(int milliseconds);

    public:
    Timer();
    void set_timer(std::function<void()> task, int milliseconds);
    void start();
    void stop();
};

#endif //TIMER_H
