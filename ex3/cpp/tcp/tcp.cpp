#include <iostream>
#include <string>
#include <unistd.h>
#include <cstring>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <errno.h>

using namespace std;

int connectTo(int port, string ip) {
    char ipc[ip.size() + 1];
    strcpy(ipc, ip.c_str());
    int sock;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        cout << "Error, socket creation failed" << endl;
        return -1;
    }

    struct sockaddr_in servaddr;
    struct hostent *hp;

    memset((char*)&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    servaddr.sin_addr.s_addr = inet_addr(ipc);

    if (connect(sock, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        cout << "Error, connection failed" << endl;
        return -1;
    }
}

void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main() {
    int port = 33546;
    string ip = "10.100.23.242";
 
    char ipc[ip.size() + 1];
    strcpy(ipc, ip.c_str());
    int sock;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        cout << "Error, socket creation failed" << endl;
        return -1;
    }

    struct sockaddr_in servaddr;

    memset((char*)&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    servaddr.sin_addr.s_addr = inet_addr(ipc);
 
    if (connect(sock, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        cout << "Error, connection failed" << endl;
        return -1;
    }
    char buf[1024];
    int numbytes = recv(sock, buf, 1023, 0);

    buf[numbytes] = '\0';
    printf("Received, %s\n", buf);

    send(sock, "Hello, world!", 14, 0);

    numbytes = recv(sock, buf, 1023, 0);
    buf[numbytes] = '\0';

    printf("Received, %s\n", buf);


    int sockacc = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in accaddr;
    memset((char*)&servaddr, 0, sizeof(servaddr));
    accaddr.sin_family = AF_INET;
    accaddr.sin_port = htons(20003);
    accaddr.sin_addr.s_addr = 0;
    int yes = 1;
    if (bind(sockacc, (struct sockaddr *) &accaddr, sizeof(accaddr)) < 0) {
        cout << "Bind failed: " << errno << endl;
        return -1;
    }   
    if (listen(sockacc, 10) < 0) {
        cout << "Failed to listen: " << errno << endl;
        return -1;
    }
    send(sock, "Connect to: 10.100.23.142:20003", 33, 0);
    
    struct sockaddr_storage addrstr;
    socklen_t sin_size = sizeof addrstr;
    int con;
    if ((con = accept(sockacc, (struct sockaddr *) &addrstr, &sin_size)) < 0) {
        cout << "Error, accepting failed: " << errno << endl;
        return -1;
    }
    char rec[INET6_ADDRSTRLEN];
    inet_ntop(addrstr.ss_family, get_in_addr((struct sockaddr*)&addrstr), rec, sizeof rec);
    printf("Connection accepted from %s\n", rec);
    if ((send(con, "Hi there!", 10, 0)) < 0) {
        cout << "Sending failed: " << errno << endl;
        return -1;
    }
    numbytes = recv(con, buf, 1023, 0);
    buf[numbytes] = '\0';

    printf("Received, %s\n", buf); 
    numbytes = recv(con, buf, 1023, 0);
    buf[numbytes] = '\0';

    printf("Received, %s\n", buf);
    close(sock);
    close(sockacc);

    return 0;
}
