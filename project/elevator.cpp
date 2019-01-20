#include "TestThread.hpp"
#include <iostream>
#include <sstream>
#include <thread>

using namespace std;

int main() {
/*    cout << "Hello world!" << endl;
    Queue q;
    auto sender = [](int msgId, int count, Queue& q) {
        for (int i = 0; i < count; i++) {
            stringstream ss;
            ss << "Message: " << i << " id: " << msgId;
            string text(ss.str());
            cout << "Send message: " << text << endl;
            q.put(DataMessage<string>(msgId, text));
        }
    };
    auto receiver = [](int count, Queue& q) {
        for (int i = 0; i < count; i++) {
            auto m = q.get();
            auto& dm = dynamic_cast<DataMessage<string>&>(*m);
            string text = dm.getPayload();
            cout << "Received message: " << text << endl;
            this_thread::sleep_for(0.2s);
        }
    };
    thread t1(sender, 1, 10, std::ref(q));
    thread t2(sender, 2, 20, std::ref(q));
    thread t3(receiver, 50, std::ref(q));
    thread t4(sender, 1, 20, std::ref(q));
    t1.join();
    t2.join();
    t3.join();
    t4.join(); */

    TestThread receiver("RecThread");
    receiver.createThread();
    for (int i = 0; i < 20; i++) {
        stringstream ss;
        ss << "Message: " << i;
        string text(ss.str());
        cout << "Send message: " << text << endl;
        receiver.getQueue().put(DataMessage<string>(MSG_STRING_DATA, text));
    }
    this_thread::sleep_for(3s);
    receiver.getQueue().put(DataMessage<string>(MSG_STRING_DATA, "Last message"));
    receiver.getQueue().put(Message(MSG_EXIT_THREAD));
    receiver.join();
    return 0;
}
