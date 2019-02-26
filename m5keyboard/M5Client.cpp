// 2019/2/21 by cp

#include <WiFi.h>
#include <HTTPClient.h>
#include "M5Client.h"
#include "M5Server.h"

bool isClientConnected=false;

void TheClient::StartHTTPClient(String ssid,String passwd){
    WiFi.begin(ssid.c_str(), passwd.c_str());
    WiFi.mode(WIFI_MODE_STA);
    isClientConnected=true;
    CheckServerTask *checkservertask= new CheckServerTask();
    checkservertask->start();
}

void RequestTask::setRequest(String urlstr,String querystr,void (*func)(int code,String payload)){
    url=urlstr;
    query=querystr;
    callback=func;
}

RequestTask::RequestTask(String url,String query,void (*func)(int code,String payload)){
    setRequest(url,query,func);
}

void RequestTask::run(void*){
 if(isClientConnected) {
        HTTPClient http;
        http.begin(url+"?"+query); //HTTP
        // start connection and send HTTP header
        int httpCode = http.GET();
        // httpCode will be negative on error
        if(httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            // file found at server
            if(httpCode == HTTP_CODE_OK) {
                String payload = http.getString();
                callback(httpCode,payload);
            }
        } else {
            callback(httpCode,"error");
        }

        http.end();
    }
}
