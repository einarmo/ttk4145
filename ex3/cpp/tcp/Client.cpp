#include "Client.hpp"
#include <iostream>
#include <cstring>
#include <unistd.h>

using namespace std;

Client::Client(string name) : BaseThread(name), recAddr(inet_addr("255.255.255.255")) {}

void Client::handleMessage(Message& msg) {
    switch(msg.getMessageId()) {
        case SET_RECEIVER_ADDR:
            {
                auto& dmsg = dynamic_cast<DataMessage<in_addr_t>&>(msg);
                recAddr = dmsg.getPayload();
                break;
            }
        case SEND_MSG:
            {
                auto& dmsg = dynamic_cast<DataMessage<string>&>(msg);
                string message = dmsg.getPayload();
                sendMessage(message);
                break;
            }
        default:
            cout << "Unknown id" << endl;
    }
}

void Client::sendMessage(string msg) {
    cout << "Sending message " << msg << endl;
    struct hostent *hp;
    struct sockaddr_in servaddr;
    memset((char*)&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(recPort);
    servaddr.sin_addr.s_addr = recAddr;

    char message[msg.size() + 1];
    strcpy(message, msg.c_str());

    int sock;
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        cout << "Error, socket creation failed" << endl;
        return;
    }

    /* if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        cout << "Error, bind failed" << endl;
        return;
    } */

    if (sendto(sock, message, strlen(message), 0, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        cout << "Error, send failed" << endl;
        return;
    }
    
}
