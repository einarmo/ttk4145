#include "WorkerThread.h"

class TestThread : public WorkerThread {
    public:
        TestThread(const char* threadName);
        void sendMessage(string text);
    private:
        void handleMessage(ThreadMessage* msg);
};


