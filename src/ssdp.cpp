#include "ssdp.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <string>

#include <fstream>
#include <cstdio>
#include <iostream>
#include <algorithm>

#include "upnpUtils.h"

const char* NOTIFY_HEADER = "NOTIFY * HTTP/1.1\r\n";
const char* MSEARCH_HEADER = "M-SEARCH * HTTP/1.1\r\n";

SSDPSocket::SSDPSocket(){
    multisock = socket(AF_INET, SOCK_DGRAM, 0);
    int sockVal = 1;
    int ret = setsockopt(multisock, SOL_SOCKET, SO_REUSEADDR, &sockVal, sizeof(int)); 

    inet_pton(AF_INET, "239.255.255.250", &multiaddr.sin_addr.s_addr);
    multiaddr.sin_port = htons(1900);
    multiaddr.sin_family = AF_INET;
    if(multisock == -1 || ret == -1){
        printf("error create socket");
        close(multisock);
        multisock = -1;
    }
    if(bind(multisock, (sockaddr *)&multiaddr, sizeof(multiaddr)) < 0)
    {
        printf("bind error\n");
    }
}
SSDPSocket::~SSDPSocket(){
    close(multisock);
}

int SSDPSocket::searchGateways(){
    std::string message(MSEARCH_HEADER);
    message.append("HOST: 239.255.255.250:1900\r\n"
		"ST: urn:schemas-upnp-org:device:InternetGatewayDevice:1\r\n"
		"MAN: \"ssdp:discover\"\r\n"
		"MX:1\r\n\r\n");
    std::cout << message;
    if(sendto(multisock, message.c_str(), strlen(message.c_str()), 0, 
    reinterpret_cast<const sockaddr *>(&multiaddr), sizeof(sockaddr_in)) < 0){
        perror("sendto");
        return -1;
    }
    char buf[256];


    int inSock = socket(AF_INET, SOCK_DGRAM, 0);
    sockaddr_in si_me{0};
    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(1900);
    si_me.sin_addr.s_addr = inet_addr("192.168.7.60");
    bind(inSock, (struct sockaddr*)&si_me, sizeof(si_me));   
    gtwEndPoints = upnpUtils::readSock(inSock);
    std::for_each(gtwEndPoints.cbegin(), gtwEndPoints.cend(), [](const std::string &str){
        std::cout << str << std::endl;
    });
    return 0;
}