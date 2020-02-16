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
// Be aware that this action will run in the epoll thread
// and thus should only add the actual work to some queue.
void Timer::set_timer(function<void()> task, int milliseconds) {
    timeouts.insert(make_pair(create_event(milliseconds), task));
}

// Start timer thread
// Main handling is currently O(recieved events * hashmap lookup)
void Timer::start() {
    timer_thread = thread([this] {
        bool should_stop = false;
        while (!should_stop) {
            // poll & block yourself
            auto event_count = epoll_wait(epoll_fd, events.data(), events.size(), -1);
            // Process received events
            for (int i = 0; i < event_count; i++) {
                auto fd = events[i].data.fd;
                cout << "Got event " << fd << endl;
                // Delete in any case
                // If it is invalid it does not concern us
                epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, nullptr);
                if (fd == stop_fd.load()) {
                    // received stop signal, end thread
                    // set a flag so we can continue processing events
                    cout << "Received stop flag" << endl;
                    should_stop = true;
                } else {
                    // Get task and if valid, perform it.
                    // Tasks from Workers class will run Workers::post()
                    // Do not put heavy computations here.
                    auto task = timeouts[fd];
                    if (task) {
                        timeouts.erase(fd);
                        task();
                    }
                }
            }
        }
    });
}

// Stop timer thread
void Timer::stop() {
    // Cheesy way: Set file descriptor to check against to an added event...
    stop_fd.store(create_event(1000));
    // Then join thread as usual
    timer_thread.join();
    cout << "Timer had " << timeouts.size() << " events left" << endl;
}
