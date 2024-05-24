#pragma once

#include <miniupnpc.h>
 
 namespace upnpUtils{
   int sendMultiCast();
   int read(int sock);
   int receiveData(int sock, int timeout_ms);
 void printDevDescr(const UPNPDev *device);
 };//upnpUtils