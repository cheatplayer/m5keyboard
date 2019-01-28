/*
    2019/1/28 by cp
*/

#include <M5Stack.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiClient.h>
#include "WebServer.h"
#include <Preferences.h>
#include "Server.h"

bool isSTAConnected=false;
bool isSTAStarted=false;
bool isAPStarted=false;

const IPAddress apIP(192, 168, 13, 1);
const char* apSSID = "M5 Wifi";
std::string wifi_ssid;
std::string wifi_password;
std::string ssidstr;

// DNSServer dnsServer;
WebServer webServer;

// wifi config store
Preferences preferences;
std::vector<std::string> ssidlist;

void Server::displayServerStatus(){
  if(isSTAConnected){
    M5.Lcd.fillCircle(30,230,3,GREEN);
    return;
  }
  if(isAPStarted){
    M5.Lcd.fillCircle(30,230,3,BLUE);
    return;
  }
  if(isSTAStarted){
    M5.Lcd.fillCircle(30,230,3,YELLOW);
    return;
  }
  M5.Lcd.fillCircle(30,230,3,RED);
}


std::string Server::makePage(std::string title, std::string contents) {
  std::string s = "<!DOCTYPE html><html><head>";
  s += "<meta name=\"viewport\" content=\"width=device-width,user-scalable=0\">";
  s += "<title>";
  s += title;
  s += "</title></head><body>";
  s += contents;
  s += "</body></html>";
  return s;
}

std::string Server::urlDecode(std::string input) {
  std::string s = input;
  s.replace("%20", " ");
  s.replace("+", " ");
  s.replace("%21", "!");
  s.replace("%22", "\"");
  s.replace("%23", "#");
  s.replace("%24", "$");
  s.replace("%25", "%");
  s.replace("%26", "&");
  s.replace("%27", "\'");
  s.replace("%28", "(");
  s.replace("%29", ")");
  s.replace("%30", "*");
  s.replace("%31", "+");
  s.replace("%2C", ",");
  s.replace("%2E", ".");
  s.replace("%2F", "/");
  s.replace("%2C", ",");
  s.replace("%3A", ":");
  s.replace("%3A", ";");
  s.replace("%3C", "<");
  s.replace("%3D", "=");
  s.replace("%3E", ">");
  s.replace("%3F", "?");
  s.replace("%40", "@");
  s.replace("%5B", "[");
  s.replace("%5C", "\\");
  s.replace("%5D", "]");
  s.replace("%5E", "^");
  s.replace("%5F", "-");
  s.replace("%60", "`");
  return s;
}

void Server::startServer(){
    webServer(80);
    webServer.on("/settings", []() {
      std::string s = "<h1>Wi-Fi Settings</h1><p>Please enter your password by selecting the SSID.</p>";
      s += "<form method=\"get\" action=\"setap\"><label>SSID: </label><select name=\"ssid\">";
      s += ssidList;
      s += "</select><br>Password: <input name=\"pass\" length=64 type=\"password\"><input type=\"submit\"></form>";
      webServer.send(200, "text/html", Server::makePage("Wi-Fi Settings", s));
    });

    webServer.on("/setap", []() {
      std::string ssid = Server::urlDecode(webServer.arg("ssid"));
      std::string pass = Server::urlDecode(webServer.arg("pass"));

      // Store wifi config
      preferences.putString("WIFI_SSID", ssid);
      preferences.putString("WIFI_PASSWD", pass);

      std::string s = "<h1>Setup complete.</h1><p>device will be connected to \"";
      s += ssid;
      s += "\" after the restart.";
      webServer.send(200, "text/html", Server::makePage("Wi-Fi Settings", s));
    });


    webServer.onNotFound([]() {
      std::string s = "<h1>AP mode</h1><p><a href=\"/settings\">Wi-Fi Settings</a></p>";
      webServer.send(200, "text/html", Server::makePage("AP mode", s));
    });

    webServer.on("/", []() {
      std::string s = "<h1>STA mode</h1><p><a href=\"/reset\">Reset Wi-Fi Settings</a></p>";
      webServer.send(200, "text/html", Server::makePage("STA mode", s));
    });
    webServer.on("/reset", []() {
      // reset the wifi config
      preferences.remove("WIFI_SSID");
      preferences.remove("WIFI_PASSWD");
      String s = "<h1>Wi-Fi settings was reset.</h1><p>Please reset device.</p>";
      webServer.send(200, "text/html", Server::makePage("Reset Wi-Fi Settings", s));
    });

    webServer.begin();
}

void Server::startAP(){
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
    WiFi.softAP(apSSID);
    WiFi.mode(WIFI_MODE_AP);
    isAPStarted=true;
    Server::displayServerStatus();
}

void Server::stopAP(){
    WiFi.mode(WIFI_MODE_AP);
    Wifi.softAPdisconnect();
    isAPStarted=false;
    Server::displayServerStatus();
}

void Server::startSTA(){
    wifi_ssid = preferences.getString("WIFI_SSID");
    if(wifi_ssid.length() > 0) {
        isSTAStarted=true;
    } else {
        isSTAStarted=false;
    }
    wifi_password = preferences.getString("WIFI_PASSWD");
    WiFi.begin(wifi_ssid.c_str(), wifi_password.c_str());
    Wifi.mode(WIFI_MODE_STA);
    Server::displayServerStatus();
}

void Server::stopSTA(){
    WiFi.mode(WIFI_MODE_STA);
    WiFi.disconnect();
    isSTAStarted=false;
    isSTAConnected=false;
    Server::displayServerStatus();
}

void Server::scanNetworks(){
    WiFi.mode(WIFI_MODE_STA);
    WiFi.disconnect();
    delay(100);
    int n = WiFi.scanNetworks();
    delay(100);
    for(int i=0;i<n;++i){
        ssidlist.push_back(WiFi.SSID(i));
        ssidstr += "<option value=\"";
        ssidstr += WiFi.SSID(i);
        ssidstr += "\">";
        ssidstr += WiFi.SSID(i);
        ssidstr += "</option>";
    } 
}

void CheckServerTask::run(void *data){
    int count = 0;
    while ( count < 30 ) {
        if (WiFi.status() == WL_CONNECTED) {
            count=30;
            isSTAConnected=true;
        }
        count++;
        vTaskDelay(1000);
    }
    Server::displayServerStatus();
}

