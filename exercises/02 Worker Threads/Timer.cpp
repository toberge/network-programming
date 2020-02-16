#include <sys/epoll.h>
#include <sys/timerfd.h>
#include <vector>
#include "Timer.h"
#include <functional>

#include <iostream>

using namespace std;

// Initializes timer and its file descriptors
Timer::Timer() : epoll_fd(epoll_create1(0)), stop_fd(-1) {
    events = vector<epoll_event>(256);
}

// Create epoll event, return file descriptor
int Timer::create_event(int milliseconds) {
    // Create event object
    epoll_event timeout;
    timeout.events = EPOLLIN;
    timeout.data.fd = timerfd_create(CLOCK_MONOTONIC, 0);

    // Timerspec - delay and possible interval
    itimerspec spec;
    spec.it_value.tv_sec = milliseconds / 1000;
    spec.it_value.tv_nsec = (milliseconds % 1000) * 1000000; // keeping cutoff from ms to sec
    // Possible interval, unset atm
    spec.it_interval.tv_sec = 0;
    spec.it_interval.tv_nsec = 0;

    // Add timespec to event
    timerfd_settime(timeout.data.fd, 0, &spec, nullptr);
    // Add this timeout to epoll itself
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, timeout.data.fd, &timeout);
    return timeout.data.fd;
}

// Sets an action to run after an amount of time
void Timer::set_timer(function<void()> task, int milliseconds) {
    timeout_event event = { task, create_event(milliseconds) };
    timeouts.emplace_back(event);
}

// Start timer thread
// Main handling is currently O(recieved events * pending timeouts)
void Timer::start() {
    timer_thread = thread([this] {
        bool should_stop = false;
        while (!should_stop) {
            cout << "Checking, " << stop_fd.load() << endl;
            // poll & block yourself
            auto event_count = epoll_wait(epoll_fd, events.data(), events.size(), -1);
            for (int i = 0; i < event_count; i++) {
                cout << "Got event " << events[i].data.fd << endl;
                if (events[i].data.fd == stop_fd.load()) {
                    // received stop signal, end thread
                    // set a flag so we can continue processing events
                    cout << "Received stop flag" << endl;
                    should_stop = true;
                    //return;
                } else {
                    //for (auto &event : timeouts) {
                    for (auto it = timeouts.begin(); it != timeouts.end();) {
                        auto event = *it;
                        if (events[i].data.fd == event.fd) {
                            // TODO add task to thing here or as task()?
                            epoll_ctl(epoll_fd, EPOLL_CTL_DEL, event.fd, nullptr);
                            event.task();
                            // Remove event from list
                            cout << "Doing this..." << endl;
                            timeouts.erase(it++);
                        } else {
                            it++;
                        }
                    }
                }
            }
        }
    });
}

// Stop timer thread
void Timer::stop() {
    // Cheesy way: Set file descriptor to check against to an added event...
    cout << "Stopping timer..." << endl;
    stop_fd.store(create_event(1000));
    // Then join thread as usual
    timer_thread.join();
    cout << "Timer had " << timeouts.size() << " events left" << endl;
}

// int main() {
//     Timer timer;
//     timer.start();
//     timer.set_timer([] {
//         cout << "Hello from wherever 3" << endl;
//     }, 3000);
//     timer.set_timer([] {
//         cout << "Hello from wherever 2" << endl;
//     }, 2000);
//     this_thread::sleep_for(1.5s);
//     timer.stop();
// }
