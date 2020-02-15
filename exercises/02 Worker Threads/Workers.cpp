#include <thread>
#include <mutex>
#include <condition_variable>
#include <list>
#include <vector>
#include <functional>
#include <iostream>

using namespace std;

/**
 * Handles a given number of worker threads
 */
class Workers {
    private:
    bool debug;
    // Thread storage
    vector<thread> thread_pool;
    // Locked by task_mutex
    bool should_stop = false;
    list<function<void()>> tasks;
    // Mutex and CV
    mutex task_mutex;
    condition_variable task_cv;

    public:
    // Create a given number of worker threads
    Workers(int num_threads, bool should_debug) {
        debug = should_debug;
        for (int i = 0; i < num_threads; i++) {
            // just add thread lambda directly
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
                            if (should_stop) {
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
    }

    // Delegating constructor for non-debug mode
    Workers(int num_threads) : Workers(num_threads, false) {}

    // Add task to queue and signal to one worker thread
    void post(function<void()> task) {
        {
            unique_lock<mutex> lock(task_mutex);
            tasks.emplace_back(task);
        }
        task_cv.notify_one();
    }

    // Wait for all threads to finish then stop & join 'em
    void stop() {
        // TODO stop threads
        {
            unique_lock<mutex> lock(task_mutex);
            should_stop = true;
            if (debug) cout << "Notified all threads" << endl;
        }
        // Broadcast to all threads
        task_cv.notify_all();
        for (auto &thread : thread_pool) {
            thread.join();
        }
        if (debug) cout << "Finished, " << tasks.size() << " tasks left undone." << endl;
    }
};

int main() {
    Workers workers(10, true);
    Workers event_loop(1);

    // Add some work
    for (int i = 0; i < 40; i++) {
        workers.post([i] {
            this_thread::sleep_for(1s);
            //cout << "Hi, I am the " << i << endl;
        });
    }

    event_loop.post([] {
        cout << "Hello from event loop" << endl;
    });

    this_thread::sleep_for(10s);

    workers.stop();
}
