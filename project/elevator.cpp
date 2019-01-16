#include <iostream>
#include <sstream>
#include "WorkerThread.h"

using namespace std;

WorkerThread testThread1("Test1");
WorkerThread testThread2("Test2");

int main() {

    testThread1.createThread();
    testThread2.createThread();

    string msg1 = "Hello world!";
    string msg2 = "Goodbye world!";
    string msg3 = "More world!";
    testThread2.postMessage(2, &msg1);
    testThread1.postMessage(2, &msg2);
    testThread2.postMessage(2, &msg3);
    
    string* arr = new string[10];

    for (int i = 0; i < 10; i++) {
        std::stringstream msg;
        msg << "Hello world: " << i;
        arr[i] = msg.str();
        testThread1.postMessage(2, &arr[i]);
    }
    this_thread::sleep_for(1s);
    testThread1.exitThread();
    testThread2.exitThread();
    return 0;
}
