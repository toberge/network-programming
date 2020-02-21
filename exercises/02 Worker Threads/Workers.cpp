#include <thread>
#include <mutex>
#include <condition_variable>
#include <list>
#include <vector>
#include <functional>
#include <iostream>
#include "Timer.h"
#include "Timer.cpp"
#include <atomic>

using namespace std;

/**
 * Handles a given number of worker threads
 */
class Workers {
    private:
    const bool debug;
    const int num_threads;
    // epoll timer thread
    Timer timer;
    // Worker thread storage
    vector<thread> thread_pool;
    // Locked by task_mutex
    atomic<bool> should_stop;
    list<function<void()>> tasks;
    // Mutex and CV
    mutex task_mutex;
    condition_variable task_cv;

    public:
    // Create a given number of worker threads (set debug to true if desired)
    Workers(int num_threads, bool debug) : debug(debug), num_threads(num_threads) {
        // Note to self: Initializers must be called in same order as the members are written
        if (num_threads < 1) {
            cerr << "Invalid number of threads" << endl;
        }
        should_stop.store(false);
        //this->debug = debug;
        //this->num_threads = num_threads;
    }

    // Create a given number of worker threads
    Workers(int num_threads) : Workers(num_threads, false) {}

    // Add task to queue and signal to one worker thread
    void post(function<void()> task) {
        {
            // Lock and add task
            unique_lock<mutex> lock(task_mutex);
            // Check if threads have been stopped
            if (should_stop.load()) {
                cerr << "Cannot add work when stop() has been called" << endl;
            }
            tasks.emplace_back(task);
        }
        // Notify to one waiting thread that there's more work available
        task_cv.notify_one();
    }

    // Performs task after a given amount of milliseconds
    // Does not guarantee that the task will be timed exactly,
    // due to the way a typical OS scheduler works.
    void post_timeout(function<void()> task, int milliseconds) {
        // TODO decrappify this solution
        // thread t([this, &task, &milliseconds] {
        //     this_thread::sleep_for(chrono::milliseconds(milliseconds));
        //     cout << "Waited" << endl;
        //     post(task);
        //     //this_thread::yield();
        // });
        timer.set_timer([this, task] {
            post(task);
        }, milliseconds);
    }

    // Called *once* to start all worker threads
    void start() {
        if (!thread_pool.empty()) {
            cerr << "Workers::start() cannot be called more than once!" << endl;
            return;
        }

        for (int i = 0; i < num_threads; i++) {
            // Add thread lambda directly
            thread_pool.emplace_back([this, i] {
                // Loop forever - until threads are explicitly stopped
                while (true)
                {
                    function<void()> task;
                    {
                        unique_lock<mutex> lock(task_mutex);
                        // Wait for signalling while no work is available
                        while (tasks.empty()) {
                            // Stop completely if stop flag is set
                            // and no work is available
                            if (should_stop.load()) {
                                if (debug) cout << "Thread " << i << " stops." << endl;
                                return;
                            }
                            // Else, wait for signals
                            task_cv.wait(lock);
                            // Waking up if work is added or stop() is called
                        }
                        if (debug) cout << "Thread " << i << " got something to do" << endl;
                        // when notified, pop off a task and perform it
                        task = *tasks.begin(); // make a copy
                        tasks.pop_front(); // actually remove
                    }
                    // out of scope, no longer holding the lock
                    if (task) {
                        task(); // perform the work
                    }
                }
                
            });
        }
        // start timer thread
        timer.start();
    }

    // Wait for all threads to finish then stop and join them
    void stop() {
        // stop timer BEFORE all else
        timer.stop();
        {
            unique_lock<mutex> lock(task_mutex);
            // Do not proceed if stop flag already is set
            if (should_stop) {
                cerr << "Workers::stop() should only be called once!" << endl;
                return;
            }
            // Set stop flag
            should_stop.store(true);
            if (debug) cout << "Notified all threads" << endl;
        }
        // Broadcast to all threads
        task_cv.notify_all();
        // Join all threads - that is, wait for them to finish
        for (auto &thread : thread_pool) {
            thread.join();
        }
        if (debug) cout << "Finished, " << tasks.size() << " tasks left undone." << endl;
    }
};

int main() {
    Workers workers(10, true);
    Workers event_loop(1);

    workers.start();

    // Add some work
    for (int i = 0; i < 40; i++) {
        workers.post([i] {
            this_thread::sleep_for(1s);
            //cout << "Hi, I am the " << i << endl;
        });
    }

    // Add work that adds work
    for (int i = 0; i < 40; i++) {
        workers.post([&workers] {
            workers.post([] {
                this_thread::sleep_for(0.5s);
            });
        });
    }

    event_loop.post([] {
        cout << "Hello from event loop" << endl;
    });

    event_loop.post([] {
        cout << "Second hello from event loop" << endl;
    });

    event_loop.post_timeout([] {
        cout << "Hello after 3 seconds" << endl;
    }, 3000);
    event_loop.post_timeout([] {
        cerr << "Should not show up..." << endl;
    }, 21000);

    workers.post_timeout([] {
        cout << "HI after 2 seconds" << endl;
    }, 2000);

    event_loop.start();
    this_thread::sleep_for(10s);
    event_loop.stop();

    workers.stop();
}
