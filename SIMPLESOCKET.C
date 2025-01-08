#include "SIMPLESOCKET.H"
#include <iostream>
#include <cstring>
#include <unistd.h>  // für close()
#include <arpa/inet.h> // für socket Funktionen

using namespace std;

TCPclient::TCPclient() {
    sock = -1;
    port = 0;
    address = "";
}

bool TCPclient::conn(string address, int port) {
    if (sock == -1) {
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock == -1) {
            perror("Could not create socket");
            return false;
        }
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = inet_addr(address.c_str());

    if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
        perror("connect failed");
        return false;
    }

    cout << "Connected to server\n";
    return true;
}

bool TCPclient::sendData(string data) {
    if (send(sock, data.c_str(), data.size(), 0) < 0) {
        perror("Send failed");
        return false;
    }
    return true;
}

string TCPclient::receive(int size) {
    char buffer[size];
    string reply;

    if (recv(sock, buffer, sizeof(buffer), 0) < 0) {
        perror("Receive failed");
    }
    reply = buffer;
    return reply;
}

// TCPserver Methoden

TCPserver::TCPserver(int port, int maxDataSizeRecv) {
    clintListn_ = socket(AF_INET, SOCK_STREAM, 0);
    memset(&ipOfServer_, 0, sizeof(ipOfServer_));
    ipOfServer_.sin_family = AF_INET;
    ipOfServer_.sin_addr.s_addr = htonl(INADDR_ANY);
    ipOfServer_.sin_port = htons(port);
    bind(clintListn_, (struct sockaddr*)&ipOfServer_, sizeof(ipOfServer_));
    dataRecv_ = new char[maxDataSizeRecv];
}

TCPserver::~TCPserver() {
    delete[] dataRecv_;
}

void TCPserver::run() {
    listen(clintListn_, 20);
    clintConnt_ = accept(clintListn_, NULL, NULL);

    while (true) {
        memset(dataRecv_, 0, maxDataSizeRecv_);
        read(clintConnt_, dataRecv_, maxDataSizeRecv_);
        string response = myResponse(string(dataRecv_));
        dataSend_ = response.c_str();
        write(clintConnt_, dataSend_, strlen(dataSend_) + 1);
    }
    close(clintConnt_);
}

string TCPserver::myResponse(string input) {
    return "Response: " + input;
}

string TCPserver::response(string incomingMsg) {
    return myResponse(incomingMsg);
}
