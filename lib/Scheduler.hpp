#include <thread>
#include <chrono>
#include <functional>
#include <queue>
#include <thread>
#include <memory>
#include <mutex>
#include <condition_variable>

using namespace std;

class FunctionTimer {
    public:
        bool operator<(const FunctionTimer& rhs) const;
        void operator()();

        FunctionTimer(function<void()> f, chrono::system_clock::time_point& t);
        virtual ~FunctionTimer() = default;
//        FunctionTimer(const FunctionTimer&) = delete;
//        FunctionTimer& operator=(const FunctionTimer&) = delete;

        chrono::system_clock::time_point time;
    private:
//        FunctionTimer(FunctionTimer&&) = default;
//        FunctionTimer& operator=(FunctionTimer&&) = default;
        function<void()> func;
};

class Scheduler {
    public:
        Scheduler();
        virtual ~Scheduler() = default;

        void stop();
        void start();
        void scheduleIn(const chrono::system_clock::duration time, function<void()> func);
        void scheduleEvery(const chrono::system_clock::duration interval, function<void()> func);
    private:
        void threadLoop();
        void reschedule(const chrono::system_clock::duration time, function<void()> func);
        bool running;
        priority_queue<FunctionTimer> tasks;
        unique_ptr<thread> mthread;

        mutex queueMutex;
        condition_variable queueCV;

        Scheduler& operator=(Scheduler&&) = default;
        Scheduler(Scheduler&&) = default;
};


