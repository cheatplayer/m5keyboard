/*
    2019/1/28 by cp
*/

#include <M5Stack.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiClient.h>
#include "WebServer.h"
#include <Preferences.h>
#include "M5Server.h"
#include "SDCard.h"
#include "Display.h"
#include "M5Client.h"
#include "Menu.h"

bool isSTAConnected=false;
bool isSTAStarted=false;
bool isAPStarted=false;

const IPAddress apIP(192, 168, 13, 1);
const char* apSSID = "TP-LINK_M5KB";
const char* appassword = "cheatplayer";
String wifi_ssid;
String wifi_password;
String ssidstr;

// DNSServer dnsServer;
WebServer webServer(80);

// wifi config store
Preferences preferences;
extern bool isClientConnected;
extern std::string record_str;

void M5Server::displayServerStatus(){
    if(isClientConnected){
        M5.Lcd.fillCircle(10,230,3,GREEN);
    }
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

void M5Server::startServer(){
    webServer.on("/", []() {
      String s = "<h1>M5Keyboard</h1><ul>";
      s+="<li><a href=\"/settings\">settings</li>";
      s+="<li>setsta</li><li>resetsta</li>";
      s+="</ul>";
      webServer.send(200, "text/html", M5Server::makePage("STA mode", s));
    });

    webServer.on("/settings", []() {
      String s = "<h1>Wi-Fi Settings</h1><p>Please enter your password by selecting the SSID.</p>";
      s += "<form method=\"get\" action=\"setsta\"><label>SSID: </label><select name=\"ssid\">";
      s += ssidstr;
      s += "</select><br>Password: <input name=\"pass\" length=64 type=\"password\"><input type=\"submit\"></form>";
      s+="<p><a href='/'>home</a></p>";
      webServer.send(200, "text/html", M5Server::makePage("Wi-Fi Settings", s));
    });

    webServer.on("/setsta", []() {
      String ssid = M5Server::urlDecode(webServer.arg("ssid"));
      String pass = M5Server::urlDecode(webServer.arg("pass"));

      // Store wifi config
      preferences.putString("WIFI_SSID", ssid);
      preferences.putString("WIFI_PASSWD", pass);

      String s = "<h1>Set STA</h1><p>Set STA ok.Please restart M5Stack.</p><p>SSID:";
      s += ssid;
      s += "</p>";  
      s+="<p><a href='/'>home</a></p>";
      webServer.send(200, "text/html", M5Server::makePage("Set STA", s));
    });

    webServer.on("/resetsta", []() {
      // rmsta the wifi config
      preferences.remove("WIFI_SSID");
      preferences.remove("WIFI_PASSWD");
      String s = "<h1>Reset STA</h1><p>Reset STA ok.Please restart M5Stack</p>";
      s+="<p><a href='/'>home</a></p>";
      webServer.send(200, "text/html", M5Server::makePage("Reset STA", s));
    });

    webServer.on("/sd",[](){
        String s="<h1>SD Card</h1>";
        std::vector<std::string> findfiles;
        findfiles=SDCard::ls("/");
        int i=0;
        //ls
        s+="<ul>";
        while(i<findfiles.size()){
            s+="<li>"+String(findfiles[i].c_str())+"</li>";
            i++;
        }
        s+="</ul>";
        //load
        s+="<h3>Load</h3><form method='get' action='/main'>file:";
        s+=M5Server::optionSD(findfiles);
        s+="<input type='submit'></form>";
        //save
        s+="<h3>Save</h3><form method='get' action='/sdsave'>file:";
        s+="<textarea name='text'></textarea>";
        s+="<input name='filename'>";
        s+="<input type='submit'></form>";
        //rm
        s+="<h3>RM</h3><form method='get' action='/sdrm'>file:";
        s+=M5Server::optionSD(findfiles);
        s+="<input type='submit'></form>";
        //back
        s+="<p><a href='/'>home</a></p>";
        webServer.send(200,"text/html",M5Server::makePage("SD Card",s));
    });

    webServer.on("/sdrm",[](){
        String filename = M5Server::urlDecode(webServer.arg("filename"));
        bool result=SDCard::rm(filename.c_str());
        if(result){
            webServer.send(200,"text/html",M5Server::jumpPath("/sd"));
        }else{
            webServer.send(200,"text/html",M5Server::errorPage("RM","rm "+filename+" fail."));
        };
    });

    webServer.on("/main",[](){
        String filename = M5Server::urlDecode(webServer.arg("filename"));

        String s="<h1>M5Keyboard Main</h1>";
        std::vector<std::string> findfiles;
        findfiles=SDCard::ls("/");

        //main
        s+="<form method='get' name='load' action='/main'></form>";
        s+="<form method='get' name='save' action='/sdsave'></form>";
        s+="<form method='get' name='run' action='/run'>file:"+filename;
        s+=M5Server::optionSD(findfiles);
        String loadfile=String(SDCard::read(filename.c_str()).c_str());
        s+="<textarea name='text'>"+loadfile+"</textarea>";
        s+="<input type='submit' form='load' value='load'>";
        s+="<input type='submit' form='save' value='save'>";
        s+="<input type='submit' form='run' value='run'>";
        s+="</form>";

        webServer.send(200,"text/html",M5Server::makePage("M5Keyboard Main",s));
    });

    webServer.on("/run",[](){
        String filename = M5Server::urlDecode(webServer.arg("filename"));
        String text = M5Server::urlDecode(webServer.arg("text"));

        const char *temp = const_cast<char*>(text.c_str());
        record_str=temp;
        Menu::runScript();

        String s="<h1>Running</h1>";
        s+="<h3>"+filename+"</h3>";
        s+="<pre>"+text+"</pre>";

        webServer.send(200,"text/html",M5Server::makePage("Running",s));
    });

    webServer.on("/sdsave",[](){
        String filename = M5Server::urlDecode(webServer.arg("filename"));
        String text = M5Server::urlDecode(webServer.arg("text"));
        bool result=SDCard::write(filename.c_str(),text.c_str());
        if(result){
            webServer.send(200,"text/html",M5Server::jumpPath("/sd"));
        }else{
            webServer.send(200,"text/html",M5Server::errorPage("Save","<p>save "+filename+" fail.</p><pre>"+text+"</pre>"));
        }
    });

    webServer.onNotFound([]() {
      String s = "<h1>404 Not Found</h1><p><a href=\"/\">home</a></p>";
      webServer.send(200, "text/html", M5Server::makePage("404 Not Found", s));
    });

    webServer.begin();
}


void M5Server::startAP(){
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
    WiFi.softAP(apSSID,appassword);
    WiFi.mode(WIFI_MODE_AP);
    isAPStarted=true;
    Display::result("192.168.13.1");
    M5Server::displayServerStatus();
}

void M5Server::stopAP(){
    WiFi.mode(WIFI_MODE_AP);
    WiFi.softAPdisconnect();
    isAPStarted=false;
    M5Server::displayServerStatus();
}

void M5Server::startSTA(){
    wifi_ssid = preferences.getString("WIFI_SSID");
    if(wifi_ssid.length() > 0) {
        isSTAStarted=true;
    } else {
        isSTAStarted=false;
    }
    wifi_password = preferences.getString("WIFI_PASSWD");
    WiFi.begin(wifi_ssid.c_str(), wifi_password.c_str());
    WiFi.mode(WIFI_MODE_STA);
    M5Server::displayServerStatus();
}

void M5Server::stopSTA(){
    WiFi.mode(WIFI_MODE_STA);
    WiFi.disconnect();
    isSTAStarted=false;
    isSTAConnected=false;
    M5Server::displayServerStatus();
}

void M5Server::scanNetworks(){
    WiFi.mode(WIFI_MODE_STA);
    WiFi.disconnect();
    delay(100);
    int n = WiFi.scanNetworks();
    delay(100);
    for(int i=0;i<n;++i){
        ssidstr += "<option value=\"";
        ssidstr += WiFi.SSID(i);
        ssidstr += "\">";
        ssidstr += WiFi.SSID(i);
        ssidstr += "</option>";
    } 
}

String M5Server::optionSD(std::vector<std::string> findfiles){
    String result="<select name='filename'>";
            int i=0;
        while(i<findfiles.size()){
            String filename=String(findfiles[i].c_str());
            result+="<option value='"+filename+"'>"+filename+"</option>";
            i++;
        }
        result+="</select>";
        return result;
}

void CheckServerTask::run(void *data){
    int count = 0;
    while ( count < 30 ) {
        if (WiFi.status() == WL_CONNECTED) {
            isSTAConnected=true;
            break;
        }
        count++;
        vTaskDelay(1000);
    }
    M5Server::displayServerStatus();
}

String M5Server::makePage(String title, String contents) {
  String s = "<!DOCTYPE html><html><head>";
  s += "<meta name=\"viewport\" content=\"width=device-width,user-scalable=0\">";
  s += "<title>";
  s += title;
  s += "</title></head><body>";
  s += contents;
  s += "</body></html>";
  return s;
}

String M5Server::jumpPath(String path) {
  String s = "<!DOCTYPE html><html><head>";
  s += "<meta name=\"viewport\" content=\"width=device-width,user-scalable=0\">";
  s += "<title>jump</title></head>";
  s += "<script language='javascript' type='text/javascript'>window.location.href='"+path+"'</script>";
  s += "<body>ok jump</body></html>";
  return s;
}

String M5Server::errorPage(String code,String msg){
  String s = "<!DOCTYPE html><html><head>";
  s += "<meta name=\"viewport\" content=\"width=device-width,user-scalable=0\">";
  s += "<title>Error</title></head><body>";
  s += "<h1>Error "+code+"</h1><p>"+msg+"</p>";
  s += "</body></html>";
  return s;
}

String M5Server::urlDecode(String input) {
  String s = input;
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
