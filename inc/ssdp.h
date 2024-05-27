#pragma once
#include <sys/types.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <list>
#include <string>
//! SSDP protocol
class SSDPSocket{
    public:
    SSDPSocket();
    ~SSDPSocket();

    int searchGateways();
    void parseGtwResponse(const char *response);
    inline bool isValid()const {
        return multisock != -1;
    }
    static std::string getGTWPoint(const std::string &response);

    int multisock = -1;
    sockaddr_in multiaddr;
    std::list<std::string> gtwEndPoints;
};