/*
    2019/1/28 by cp
*/

#ifndef SERVER_H
#define SERVER_H
#include "Task.h"

namespace M5Server{
    void displayServerStatus();
    String makePage(String title, String contents);
    String urlDecode(String input);
    void startServer();
    void startAP();
    void stopAP();
    void startSTA();
    void stopSTA();
    void scanNetworks();
};

class CheckServerTask:public Task{
public:
    void run(void *data);
};


#endif
