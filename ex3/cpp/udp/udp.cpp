#include "Client.hpp"
#include <iostream>
#include <string>
#include <unistd.h>
#include <cstring>

using namespace std;

string receiveFrom(int port) {
    char buffer[256];
    
    // Create socket
    int sock;
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        return "Error, socket creation failed\n";
    }
    
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);
    if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        return "Error, bind failed\n";
    }

    struct sockaddr_in remaddr;
    socklen_t addrlen = sizeof(remaddr);
    
    int recvlen = 0;
    while (!recvlen) {
        recvlen = recvfrom(sock, buffer, 256, 0, (struct sockaddr *)&remaddr, &addrlen);
    }
    close(sock);
    return string(buffer);
}

int main() {
    string msg = receiveFrom(30000);
    cout << "Received message: " << msg << endl;
    size_t ipStart = msg.find("server at ");
    string ip = msg.substr(ipStart + 10, msg.length() - ipStart - 11);
    
    Client sender("Sender");
    sender.createThread();
    
    char ipArr[ip.size() + 1];

    sender.getQueue().put(DataMessage<in_addr_t>(SET_RECEIVER_ADDR,
        inet_addr(strcpy(ipArr, ip.c_str()))));
    
    sender.getQueue().put(DataMessage<string>(SEND_MSG, "Hello server"));
    msg = receiveFrom(20003);
    sender.exitThread();
    sender.join();
    cout << "Received message: " << msg << endl;
    return 0;
}
