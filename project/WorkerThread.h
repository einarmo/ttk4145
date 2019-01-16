#include <string>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;

struct ThreadMessage {
    ThreadMessage(int tp, void* m) { type = tp; msg = m; }
    int type;
    const void* msg;
};

class WorkerThread {
    public:
        WorkerThread(const char* threadName);
        
        ~WorkerThread();
        
        bool createThread();
        
        void exitThread();
        
        thread::id getThreadId();

        static thread::id getCurrentThreadId();

        void postMessage(int type, void* m);
    private:
        WorkerThread(const WorkerThread&);
        WorkerThread& operator=(const WorkerThread&);

        void process();
    protected:
        virtual void handleMessage(ThreadMessage* msg) {}
        queue<ThreadMessage*> messageQueue;
        mutex messageMutex;
        condition_variable cv;
        const char* THREAD_NAME;
        thread* activeThread;
};
