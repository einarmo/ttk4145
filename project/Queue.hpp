#include <queue>
#include <mutex>
#include <condition_variable>
#include "Message.hpp"
#include <memory>

const int MSG_TIMEOUT = -1;

using namespace std;

class Queue {
    public:
        Queue();
        ~Queue();

        void put(Message&& msg);

        unique_ptr<Message> get(int timeoutms = 0);
    private:
        queue<unique_ptr<Message>> mQueue;
        mutex queueMutex;

        condition_variable queueCV;
};
