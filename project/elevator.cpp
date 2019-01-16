#include "Queue.hpp"
#include <iostream>
#include <sstream>
#include <thread>

using namespace std;

int main() {
    cout << "Hello world!" << endl;
    Queue q;
    auto sender = [](int msgId, int count, Queue& q) {
        for (int i = 0; i < count; i++) {
            stringstream ss;
            ss << "Message: " << i << " id: " << msgId;
            string text(ss.str());
            q.put(DataMessage<string>(msgId, text));
            this_thread::sleep_for(0.03s);
        }
    };
    auto receiver = [](int count, Queue& q) {
        for (int i = 0; i < count; i++) {
            auto m = q.get();
            auto& dm = dynamic_cast<DataMessage<string>&>(*m);
            string text = dm.getPayload();
            cout << "Received message: " << text << endl;
            this_thread::sleep_for(0.02s);
        }
    };
    thread t1(sender, 1, 10, std::ref(q));
    thread t2(sender, 2, 20, std::ref(q));
    thread t3(receiver, 30, std::ref(q));
    t1.join();
    t2.join();
    t3.join();
    return 0;
}
