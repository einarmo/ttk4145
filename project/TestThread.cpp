#include "TestThread.h"
#include <iostream>

#define MSG_TEST_DATA    2

using namespace std;

TestThread::TestThread(const char* threadName) : WorkerThread(threadName) {
}

void TestThread::handleMessage(ThreadMessage* msg) {
    switch (msg->type) {
        case MSG_TEST_DATA:
            {
                const string* data = static_cast<const string*>(msg->msg);
                cout << "Received: " << *data << " on thread " << THREAD_NAME << endl;
                delete msg;
                this_thread::sleep_for(0.1s);
                break;
            }
        default:
            cout << "Bad message type" << endl;
    }
}

void TestThread::sendMessage(string text) {
    postMessage(MSG_TEST_DATA, &text);
}

