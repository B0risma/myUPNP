#include <iostream>
#include <upnpcommands.h>
#include <upnpUtils.h>
#include <ssdp.h>
#include <thread>
#include <unistd.h>
#include <ctime>
#include <iomanip>



int main(int argc, char *argv[]){
    int error = 0;
    SSDPSocket sock;
    std::thread searchTread(&SSDPSocket::searchGateways, &sock);
    while(sock.searching){
        auto tmp = std::chrono::system_clock::now().time_since_epoch();
        std::cout << tmp.count() << std::endl;
    }
    searchTread.join();
    for(auto str : sock.gtwEndPoints){
        std::cout << str << std::endl;
    }
    return 0;
 }