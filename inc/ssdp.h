#pragma once
#include <sys/types.h>

#include <arpa/inet.h>
#include <netinet/in.h>
//! SSDP protocol
class SSDPSocket{
    public:
    SSDPSocket();
    ~SSDPSocket();

    int searchGateways() const;
    inline bool isValid()const {
        return multisock != -1;
    }

    int multisock = -1;
    sockaddr_in multiaddr;
};