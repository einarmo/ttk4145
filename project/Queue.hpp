#ifndef MESSAGE_QUEUE_H_
#define MESSAGE_QUEUE_H_

#include <queue>
#include <mutex>
#include <condition_variable>
#include "Message.hpp"
#include <memory>

#define MSG_TIMEOUT -1

using namespace std;

class Queue {
    public:
        Queue();
        ~Queue();

        void put(Message&& msg);
        void put(unique_ptr<Message> msg);

        unique_ptr<Message> get(int timeoutms = 0);
    private:
        queue<unique_ptr<Message>> mQueue;
        mutex queueMutex;

        condition_variable queueCV;
};

#endif
