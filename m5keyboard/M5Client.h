// 2019/2/21 by cp

#ifndef M5CLIENT_H
#define M5CLIENT_H
#include "Task.h"

namespace TheClient{
    void StartHTTPClient(String ssid,String passwd);
    void sendClient(char key_val);
    std::string urlEncode(std::string input);
}

class RequestTask:public Task{
public:
    String url;
    String query;
    void (*callback)(int code,String payload);
    void setRequest(String urlstr,String querystr,void (*func)(int code,String payload));
    RequestTask();
    RequestTask(String url,String query,void (*func)(int code,String payload));
    RequestTask(std::string url,std::string query,void (*func)(int code,String payload));
    void run(void*);
};

#endif
