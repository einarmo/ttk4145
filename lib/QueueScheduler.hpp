#include "Queue.hpp"
#include <thread>
#include <chrono>
#include <functional>
#include <queue>
#include <thread>
#include <memory>
#include <mutex>
#include <condition_variable>

using namespace std;

class MessageTimer {
    public:
        bool operator<(const MessageTimer& rhs) const;

        MessageTimer(unique_ptr<Message> m, chrono::system_clock::time_point& t);
        virtual ~MessageTimer() = default;
        unique_ptr<Message> get();

        chrono::system_clock::time_point time;
    private:
        unique_ptr<Message> msg;
};

class Compare {
    public:
        bool operator()(const unique_ptr<MessageTimer> &lhs, const unique_ptr<MessageTimer> &rhs) {
            return lhs->time > rhs->time;
        }
};

class QueueScheduler {
    public:
        QueueScheduler(Queue &q);
        virtual ~QueueScheduler() = default;

        void stop();
        void start();
        void scheduleIn(const chrono::system_clock::duration time, Message&& msg);
    private:
        QueueScheduler();
        void threadLoop();
        bool running;

        priority_queue<unique_ptr<MessageTimer>, vector<unique_ptr<MessageTimer>>, Compare> tasks;
        Queue &mqueue;

        unique_ptr<thread> mthread;
        mutex queueMutex;
        condition_variable queueCV;

        QueueScheduler& operator=(QueueScheduler&&) = default;
        QueueScheduler(QueueScheduler&&) = default;
};


