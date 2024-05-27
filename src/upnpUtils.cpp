#include <upnpUtils.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <cstring>
#include <algorithm>
#include <vector>
#include <iostream>
#include <unistd.h>
#include <chrono>
#include <poll.h>
#include <list>
#include <string>

namespace upnpUtils{

int getMulticastSocket(int &sock){
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    int sockVal =1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &sockVal, sizeof(int));
    return 0;
}

int sendMultiCast(){
    int multiSock;
    getMulticastSocket(multiSock);
    sockaddr_in s;
    inet_pton(AF_INET, "239.255.255.250", &s.sin_addr.s_addr);
    s.sin_port = htons(1900);
    s.sin_family = AF_INET;

    bind(multiSock, (sockaddr *)&s, sizeof(s));
    auto message = "NOTIFY * HTTP/1.1\r\n";
    if(sendto(multiSock, message, strlen(message), 0, reinterpret_cast<sockaddr *>(&s), sizeof(sockaddr_in)) < 0){
        perror("sendto");
        return -1;
    }
    return 0;
}

void printDevDescr(const UPNPDev *device){
    if(!device)    return;
    printf("device: \n");
    printf("descrUrl: %s\n", device->descURL);
    printf("st: %s\n", device->st);
    printf("usn: %s\n", device->usn);
    printf("buffer: %s\n", device->buffer);
    printf("scope_id: %d\n", device->scope_id);
}


int receiveData(int sock, int timeout_ms){
    char buffer[1024];
    //auto startTime = std::chrono::system_clock::now();
    //auto timeoutEnd = startTime + std::chrono::milliseconds(timeout_ms);
    sockaddr_in si_other{0};
    socklen_t addrLen;
    //while(timeoutEnd != std::chrono::system_clock::now()){
        recvfrom(sock, buffer, 1024, 0, (struct sockaddr*)& si_other, &addrLen);
        printf("[+]Data Received: %s", buffer);
    //}
    return 0;
}
std::list<std::string> readSock(int sock){

    pollfd fds[1];
  int sockfd = sock;
  fds[0].fd = sockfd;
  fds[0].events = POLLIN;
  std::list<std::string> dataList;

  char inputBuff[1024];
    for(int ret = poll(fds, 1, 200); ret >0; ret = poll(fds,1,200)){
        if(fds[0].revents & POLLIN){
            recv(sock, inputBuff, 1024, 0);
            dataList.push_back(std::string(inputBuff));
            fds[0].revents = 0;
        }
    }
    return dataList;
}

};//upnpUtils