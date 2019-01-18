#include <thread>
#include "Queue.hpp"
#include <string>

#define MSG_EXIT_THREAD 1

using namespace std;

class BaseThread {
    public:
        BaseThread(string name);
        ~BaseThread();

        bool createThread();

        void exitThread();

        Queue& getQueue();

        void join();
    protected:
        virtual void handleMessage(Message& msg) = 0;

        void process();
    private:

        thread* mthread;
        
        Queue mqueue;

        const string THREAD_NAME;
};
