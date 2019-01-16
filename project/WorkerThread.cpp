#include "WorkerThread.h"
#include <iostream>

#define MSG_EXIT_THREAD     1


using namespace std;

WorkerThread::WorkerThread(const char* threadName)
    : activeThread(0), THREAD_NAME(threadName) {
}

WorkerThread::~WorkerThread() {
    exitThread();
}

bool WorkerThread::createThread() {
    if (!activeThread) {
        activeThread = new thread(&WorkerThread::process, this);
    }
    return true;
}

thread::id WorkerThread::getThreadId() {
    return activeThread->get_id();
}

thread::id WorkerThread::getCurrentThreadId() {
    return this_thread::get_id();
}

void WorkerThread::exitThread() {
    if (!activeThread) return;
    ThreadMessage* msg = new ThreadMessage(MSG_EXIT_THREAD, 0);
    {
        lock_guard<mutex> lock(messageMutex);
        messageQueue.push(msg);
        cv.notify_one();
    }
    activeThread->join();
    delete activeThread;
    activeThread = 0;
}

void WorkerThread::process() {
    while (1) {
        ThreadMessage* msg = 0;
        {
            unique_lock<mutex> lock(messageMutex);
            while (messageQueue.empty()) cv.wait(lock);

            if (messageQueue.empty()) continue;

            msg = messageQueue.front();
            messageQueue.pop();
        }
        switch (msg->type) {
            case MSG_EXIT_THREAD:
            {
                delete msg;
                unique_lock<mutex> lock(messageMutex);
                while (!messageQueue.empty()) {
                    msg = messageQueue.front();
                    messageQueue.pop();
                    delete msg;
                }
                cout << "Exit thread on " << THREAD_NAME << endl;
                return;
            }
            case 2:
            {
                const string* data = static_cast<const string*>(msg->msg);
                cout << "Received: " << *data << " on thread " << THREAD_NAME << endl;
                delete msg;
                this_thread::sleep_for(1s);
                break;
            }
            default:
                handleMessage(msg);
        }
    }
}

void WorkerThread::postMessage(int type, void* m) {
    if (!activeThread) { throw "No thread"; }
    ThreadMessage* msg = new ThreadMessage(type, m);
    unique_lock<mutex> lock(messageMutex);
    messageQueue.push(msg);
    cv.notify_one();
}
