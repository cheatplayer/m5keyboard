/*
    2019/1/28 by cp
*/

#ifndef SERVER_H
#define SDCARD_H

#include "Task.h"
#include <string>

namespace Server{
    void displayServerStatus();
    std::string makePage();
    std::string urlDecode();
    void startServer();
    void startAP();
    void stopAP();
    void startSTA();
    void stopSTA();
    void scanNetworks();
}

class CheckServerTask:public Task{
public:
    void run(void *data);
}


#endif