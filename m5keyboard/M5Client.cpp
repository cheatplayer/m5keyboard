// 2019/2/21 by cp

#include <WiFi.h>
#include <HTTPClient.h>
#include "M5Client.h"
#include "M5Server.h"
#include "ShExec.h"
#include "Display.h"

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

RequestTask::RequestTask(std::string url,std::string query,void (*func)(int code,String payload)){
    String turl=String()+url.c_str();
    String tquery=String()+query.c_str();
    setRequest(turl,tquery,func);
}

RequestTask::RequestTask(String url,String query,void (*func)(int code,String payload)){
    setRequest(url,query,func);
}

void RequestTask::run(void*){
 if(isClientConnected) {
        HTTPClient http;
        http.begin(url+"?"+query); //HTTP
        Serial.println(url+"?"+query);
        // start connection and send HTTP header
        int httpCode = http.POST("");
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

std::string TheClient::urlEncode(std::string input) {//replace
  if(input== " ")return "+";
  else if(input== " ")return "%20";
  else if(input== "!")return "%21";
  else if(input== "\"")return "%22";
  else if(input== "#")return "%23";
  else if(input== "%")return "%25";
  else if(input== "&")return "%26";
  else if(input== "\'")return "%27";
  else if(input== "(")return "%28";
  else if(input== ")")return "%29";
  else if(input== "+")return "%2B";
  else if(input== ".")return "%2E";
  else if(input== "/")return "%2F";
  else if(input== ",")return "%2C";
  else if(input== ":")return "%3A";
  else if(input== ",")return "%3B";
  else if(input== "<")return "%3C";
  else if(input== "=")return "%3D";
  else if(input== ">")return "%3E";
  else if(input== "?")return "%3F";
  else if(input== "@")return "%40";
  else if(input== "[")return "%5B";
  else if(input== "\\")return "%5C";
  else if(input== "]")return "%5D";
  else if(input== "^")return "%5E";
  else if(input== "_")return "%5F";
  else if(input== "`")return "%60";
  else if(input== "{")return "%7B";
  else if(input== "|")return "%7C";
  else if(input== "}")return "%7D";
  else if(input== "\r")return "%0D";
  else if(input== "\n")return "%0A";
  // s.replace("%09", "\t");
  return input;
}

void clientcallback(int code,String payload){
    Serial.println(code);
    Serial.println(payload);
    Display::result(payload.c_str());
}

void TheClient::sendClient(std::string text){
    if(isClientConnected){
        Serial.println(text);
        std::string query="q="+TheClient::urlEncode(text)+"&";
        RequestTask *reqtask;
        reqtask=new RequestTask("http://192.168.4.1/input",query,clientcallback);
        reqtask->start();
    }
}

void TheClient::sendCmd(std::string text){
    if(isClientConnected){
        Serial.println(text);
        std::string query="q="+TheClient::urlEncode(text)+"&";
        RequestTask *reqtask;
        reqtask=new RequestTask("http://192.168.4.1/cmd",query,clientcallback);
        reqtask->start();
    }
}
