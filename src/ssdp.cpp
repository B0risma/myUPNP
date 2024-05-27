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
#define UPNP_MULTICAST_ADDR "239.255.255.250"
constexpr const char* SEARCH_BODY = "HOST: "UPNP_MULTICAST_ADDR":1900\r\n"
		"ST: urn:schemas-upnp-org:device:InternetGatewayDevice:1\r\n"
		"MAN: \"ssdp:discover\"\r\n"
		"MX:1\r\n\r\n";
SSDPSocket::SSDPSocket(){
    multisock = socket(AF_INET, SOCK_DGRAM, 0);
    int sockVal = 1;
    int ret = setsockopt(multisock, SOL_SOCKET, SO_REUSEADDR, &sockVal, sizeof(int)); 
    inet_pton(AF_INET, UPNP_MULTICAST_ADDR, &multiaddr.sin_addr.s_addr);
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

std::string SSDPSocket::getGTWPoint(const std::string &response){
    std::string gtwAddress{};
    constexpr auto LOC_STR = "LOCATION:";
    auto firstPos = response.find("LOCATION:");
    auto lastPos = response.find('\r', firstPos);
    if(lastPos == std::string::npos)
        lastPos = response.find('\n', firstPos);
    if(firstPos != std::string::npos && lastPos != std::string::npos){
            gtwAddress = response.substr(firstPos, (lastPos-firstPos));
            constexpr size_t LOC_STR_SIZE = strlen(LOC_STR); 
            gtwAddress.erase(gtwAddress.begin(), gtwAddress.begin()+LOC_STR_SIZE);
            for(auto spaceIdx = gtwAddress.find(' '); spaceIdx != std::string::npos;
                    spaceIdx = gtwAddress.find(' '))
                gtwAddress.erase(spaceIdx, 1);
    }
    return gtwAddress;
    }


int SSDPSocket::searchGateways(){
    searching = true;
    std::string message(MSEARCH_HEADER);
    message+=SEARCH_BODY;
        gtwEndPoints.clear();
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
    si_me.sin_addr.s_addr = htons(INADDR_ANY);
    {
       const int reuse = 1;
       if (setsockopt(inSock, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(reuse)) < 0)
           perror("setsockopt(SO_REUSEADDR) failed");
        if (setsockopt(inSock, SOL_SOCKET, SO_REUSEPORT, (const char*)&reuse, sizeof(reuse)) < 0)
           perror("setsockopt(SO_REUSEPORT) failed");
    }
    if(bind(inSock, (struct sockaddr*)&si_me, sizeof(si_me)) < 0)
        perror("bind failure");   
    std::list<std::string> sockDatas = upnpUtils::readSock(inSock);
    for(const auto &str : sockDatas){
        const auto &gtwPoint = getGTWPoint(str);
        if(gtwPoint.empty()) continue;
        gtwEndPoints.push_back(gtwPoint);
    }
    close(inSock);
    searching = false;
    return 0;
}