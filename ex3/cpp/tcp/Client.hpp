#include "BaseThread.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>

#define SET_RECEIVER_ADDR 2
#define SEND_MSG 3

class Client : public BaseThread {
    public:
        Client(string name);
        virtual ~Client() = default;
    protected:
        void handleMessage(Message& msg);
    private:
        void sendMessage(string msg);
        in_addr_t recAddr;
        const int recPort = 20003;
};
