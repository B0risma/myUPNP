#pragma once

#include <miniupnpc.h>
#include <list>
#include <string>
 
 namespace upnpUtils{
   int sendMultiCast();
   int read(int sock);
   std::list<std::string> readSock(int sock);
   int receiveData(int sock, int timeout_ms);
 void printDevDescr(const UPNPDev *device);
 };//upnpUtils