#include "TestThread.hpp"
#include <iostream>

TestThread::TestThread(string name) : BaseThread(name) {}

using namespace std;

void TestThread::handleMessage(Message& msg) {
    switch(msg.getMessageId()) {
        case MSG_STRING_DATA:
            {
                auto& dmsg = dynamic_cast<DataMessage<string>&>(msg);
                string text = dmsg.getPayload();
                cout << "Received message: " << text << endl;
                this_thread::sleep_for(0.1s);
                break;
            }
        default:
            cout << "Unknown id" << endl;
    }
}
