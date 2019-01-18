#include "BaseThread.hpp"
#include <iostream>

using namespace std;

BaseThread::BaseThread(const string name)
    : mthread(0), THREAD_NAME(name) {}

BaseThread::~BaseThread() {
    exitThread();
}

Queue& BaseThread::getQueue() { return mqueue; }

void BaseThread::exitThread() {
    if (!mthread) return;
    mqueue.put(Message(MSG_EXIT_THREAD));
}

bool BaseThread::createThread() {
    if (mthread) return true;
    mthread = new thread(&BaseThread::process, this);
    return true;
}

void BaseThread::join() {
    if (!mthread) return;
    mthread->join();
}

void BaseThread::process() {
    while (1) {
        auto m = mqueue.get();
        Message& msg = dynamic_cast<Message&> (*m);
        switch(msg.getMessageId()) {
            case MSG_EXIT_THREAD:
                {
                    cout << "Exit thread on " << THREAD_NAME << endl;
                    return;
                }
            default:
                handleMessage(msg);
        }
    }
}

