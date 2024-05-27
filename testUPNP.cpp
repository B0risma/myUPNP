#include <iostream>
#include <miniupnpc.h>
#include <upnpcommands.h>
#include <upnpUtils.h>
#include <ssdp.h>



int main(int argc, char *argv[]){
    UPNPUrls test;
    int error = 0;
    SSDPSocket sock;
    if(sock.isValid())
        sock.searchGateways();
    for(auto str : sock.gtwEndPoints){
        std::cout << str << std::endl;
    }
    //auto devList = upnpDiscover(200, 0, 0, UPNP_LOCAL_PORT_ANY, 0, 2, &error);
    /* for(auto dev = devList; dev; dev = dev->pNext){
        upnpUtils::printDevDescr(dev);
    }
    std::cout << test.controlURL <<  "hello\n"; */
    //upnpUtils::read();
    return 0;
 }