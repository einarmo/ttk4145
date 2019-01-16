
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

        unique_ptr<Message> request(Message&& msg);
        
        void respondTo(MsgUID reqUid, Message&& responseMsg);
    private:
        class Implement;
        unique_ptr<Implement> implement;
};
