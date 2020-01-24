#include "Queue.hpp"

#include <utility>

Queue::Queue() : mQueue(), queueMutex(), queueCV() {}

Queue::~Queue() {}

void Queue::put(Message&& msg) {
    {
        lock_guard<mutex> lock(queueMutex);
        mQueue.push(msg.move());
    }
    queueCV.notify_one();
}

unique_ptr<Message> Queue::get(int timeoutms) {
    unique_lock<mutex> lock(queueMutex);
    if (timeoutms <= 0) {
        queueCV.wait(lock, [this]{ return !mQueue.empty(); });
    } else {
        auto timeoutOccured = queueCV.wait_for(lock,
                chrono::milliseconds(timeoutms),
                [this]{ return !mQueue.empty(); });
        if (timeoutOccured) {
            mQueue.emplace(new Message(MSG_TIMEOUT));
        }
    }
    auto msg = mQueue.front()->move();
    mQueue.pop();
    return msg;
}
