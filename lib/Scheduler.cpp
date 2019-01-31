#include "Scheduler.hpp"
#include <iostream>

using namespace std;

bool FunctionTimer::operator<(const FunctionTimer& rhs) const {
    return time > rhs.time;
}

void FunctionTimer::operator()() {
    func();
}

FunctionTimer::FunctionTimer(function<void()> f, chrono::system_clock::time_point& t)
    : time(t), func(f) {}

Scheduler::Scheduler() : running(false), queueMutex(), queueCV() {}

void Scheduler::stop() {
    {
        lock_guard<mutex> lock(queueMutex);
        running = false;
    }
    queueCV.notify_one();
    mthread->join();
}

void Scheduler::start() {
    running = true;
    mthread = unique_ptr<thread>(new thread(&Scheduler::threadLoop, this));
}

void Scheduler::scheduleIn(const chrono::system_clock::duration time, function<void()> func) {
    chrono::system_clock::time_point point = chrono::system_clock::now() + time;
    {
        lock_guard<mutex> lock(queueMutex);
        tasks.push(FunctionTimer(move(func), point));
    }
    queueCV.notify_one();
}

void Scheduler::reschedule(const chrono::system_clock::duration interval, function<void()> func) {
    chrono::system_clock::time_point point = chrono::system_clock::now() + interval;
    function<void()> wrapFunc = [this, interval, func]() {
        func();
        this->reschedule(interval, func);
    };
    tasks.push(FunctionTimer(move(wrapFunc), point));
}

void Scheduler::scheduleEvery(const chrono::system_clock::duration interval, function<void()> func) {
    function<void()> wrapFunc = [this, interval, func]() {
        func();
        this->reschedule(interval, func);
    };
    scheduleIn(interval, move(wrapFunc));
}

void Scheduler::threadLoop() {
    while(running) {
        unique_lock<mutex> lock(queueMutex);
        auto now = chrono::system_clock::now();
        while(!tasks.empty() && tasks.top().time <= now) {
            FunctionTimer f = tasks.top();
            f();
            tasks.pop();
        }
        if (!tasks.empty()) {
            queueCV.wait_for(lock, tasks.top().time - now,
                [this]{ return !tasks.empty() && tasks.top().time <= chrono::system_clock::now(); });
        } else {
            queueCV.wait(lock, [this]{ return !tasks.empty(); });
        }
    }
}


