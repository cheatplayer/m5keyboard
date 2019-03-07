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

String TheClient::urlEncode(String input) {
  String s=input;
  s.replace(" ","+");
  s.replace(" ","%20");
  s.replace("!","%21");
  s.replace("\"","%22");
  s.replace("#","%23");
  s.replace("%","%25");
  s.replace("&","%26");
  s.replace("\'","%27");
  s.replace("(","%28");
  s.replace(")","%29");
  s.replace("+","%2B");
  s.replace(".","%2E");
  s.replace("/","%2F");
  s.replace(",","%2C");
  s.replace(":","%3A");
  s.replace(",","%3B");
  s.replace("<","%3C");
  s.replace("=","%3D");
  s.replace(">","%3E");
  s.replace("?","%3F");
  s.replace("@","%40");
  s.replace("[","%5B");
  s.replace("\\","%5C");
  s.replace("]","%5D");
  s.replace("^","%5E");
  s.replace("_","%5F");
  s.replace("`","%60");
  s.replace("{","%7B");
  s.replace("|","%7C");
  s.replace("}","%7D");
  s.replace("\r","%0D");
  s.replace("\n","%0A");
  // s.replace("%09", "\t");
  return s;
}

void clientcallback(int code,String payload){
    Serial.println(code);
    Serial.println(payload);
    Display::result(payload.c_str());
}

void TheClient::sendClient(std::string text){
    if(isClientConnected){
        Serial.println(text.c_str());
        String t=String()+text.c_str();
        String query="q="+TheClient::urlEncode(t)+"&";
        RequestTask *reqtask;
        reqtask=new RequestTask("http://192.168.4.1/input",query,clientcallback);
        reqtask->start();
    }
}

void TheClient::sendCmd(std::string text){
    if(isClientConnected){
        Serial.println(text.c_str());
        String cmd=String()+text.c_str();
        String query="q="+TheClient::urlEncode(cmd)+"&";
        RequestTask *reqtask;
        reqtask=new RequestTask("http://192.168.4.1/cmd",query,clientcallback);
        reqtask->start();
    }
}
