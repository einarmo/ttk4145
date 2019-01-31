#include "QueueScheduler.hpp"
#include <iostream>

using namespace std;

unique_ptr<Message> MessageTimer::get() {
    return msg->move();
}

MessageTimer::MessageTimer(unique_ptr<Message> m, chrono::system_clock::time_point& t)
    : time(t), msg(m->move()) {}

QueueScheduler::QueueScheduler(Queue &q) : running(false), queueMutex(), queueCV(), mqueue(q) {}

void QueueScheduler::stop() {
    {
        lock_guard<mutex> lock(queueMutex);
        running = false;
    }
    queueCV.notify_one();
    mthread->join();
}

void QueueScheduler::start() {
    running = true;
    mthread = unique_ptr<thread>(new thread(&QueueScheduler::threadLoop, this));
}

void QueueScheduler::scheduleIn(const chrono::system_clock::duration time, Message&& msg) {
    chrono::system_clock::time_point point = chrono::system_clock::now() + time;
    {
        lock_guard<mutex> lock(queueMutex);
        tasks.push(unique_ptr<MessageTimer>(new MessageTimer(msg.move(), point)));
    }
    queueCV.notify_one();
}

void QueueScheduler::threadLoop() {
    while(running) {
        unique_lock<mutex> lock(queueMutex);
        auto now = chrono::system_clock::now();
        while(!tasks.empty() && tasks.top()->time <= now) {
            mqueue.put(tasks.top()->get()->move());
            tasks.pop();
        }
        if (!tasks.empty()) {
            queueCV.wait_for(lock, tasks.top()->time - now,
                [this]{ return !tasks.empty()
                    && tasks.top()->time <= chrono::system_clock::now() || !running; });
        } else {
            queueCV.wait(lock, [this]{ return !tasks.empty() || !running; });
        }
    }
}


