#include "BaseThread.hpp"

#define MSG_STRING_DATA 2

using namespace std;

class TestThread : public BaseThread {
    public:
        TestThread(string name);
        virtual ~TestThread() = default;
    protected:
        void handleMessage(Message& msg);
};

