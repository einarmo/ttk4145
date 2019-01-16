#include "Queue.hpp"

#include <chrono>
#include <condition_variable>
#include <queue>
#include <map>
#include <mutex>
#include <utility>

class Queue::Implement {
    public:
        Implement() : mQueue(), queueMutex(), queueCV(), responseMap(), responseMapMutex() {}
        void put(Message&& msg) {
            {
                lock_guard<mutex> lock(queueMutex);
                mQueue.push(msg.move());
            }
            queueCV.notify_one();
        }
        unique_ptr<Message> get(int timeoutms) {
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
        unique_ptr<Message> request(Message&& msg) {
            unique_lock<mutex> lock(responseMapMutex);
            auto it = responseMap.emplace(make_pair(msg.getUniqueId(),
                unique_ptr<Queue>(new Queue))).first;
            lock.unlock();

            put(std::move(msg));
            auto response = it->second->get();

            lock.lock();
            responseMap.erase(it);
            lock.unlock();

            return response;
        }
        void respondTo(MsgUID reqUid, Message&& responseMsg) {
            lock_guard<mutex> lock(responseMapMutex);
            if (responseMap.count(reqUid) < 0) {
                responseMap[reqUid]->put(std::move(responseMsg));
            }
        }
    private:
        queue<unique_ptr<Message>> mQueue;
        mutex queueMutex;

        condition_variable queueCV;

        map<MsgUID, unique_ptr<Queue>> responseMap;
        mutex responseMapMutex;
};


Queue::Queue() : implement(new Implement) {}

Queue::~Queue() {}

void Queue::put(Message&& msg) { implement->put(std::move(msg)); }

unique_ptr<Message> Queue::get(int timeoutms) {
    return implement->get(timeoutms);
}

unique_ptr<Message> Queue::request(Message&& msg) {
    return implement->request(std::move(msg));
}

void Queue::respondTo(MsgUID reqUid, Message&& responseMsg) {
    return implement->respondTo(reqUid, std::move(responseMsg));
}

