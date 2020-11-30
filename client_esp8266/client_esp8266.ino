#include <ESP8266WiFi.h>
#include <FS.h>
#include <ESP8266mDNS.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFSEditor.h>
#include <EEPROM.h>

#define BAUD_RATE 57200
#define bufferSize 600
#define debug false

/* ============= CHANGE WIFI CREDENTIALS ============= */
const char *ssid = "M5CL";
const char *password = "cheatplayer"; //min 8 chars
/* ============= ======================= ============= */

String lstext="";
String readtext="";

AsyncWebServer server(80);

bool shouldReboot = false;

void setup() {
  
  Serial.begin(BAUD_RATE);
  delay(2000);
  if(debug) sendCmd("CMSG","8266","starting SSID: "+ (String)ssid +" Password: "+ (String)password);

  EEPROM.begin(4096);
  SPIFFS.begin();
  
  WiFi.mode(WIFI_STA);
  WiFi.softAP(ssid, password,1, false);
  
  StartServer();
}

void loop() {
  if(shouldReboot) ESP.restart();
  
  if(Serial.available()) {
    uint8_t answer = Serial.read();
    if(answer == 0x04) {
      if(debug) sendCmd("CMSG","8266","8266 done");
      runLine = true;
    }
    else {
        String originStr = (char)answer + Serial.readStringUntil(255);
        if(originStr[0]=='\001') {
            String cmd=parseCmd(originStr,'\001','\002');
            String filename=parseCmd(originStr,'\002','\003');
            String data=parseCmd(originStr,'\003','\004');
            if(cmd=="READ"){
                readtext=data;
            }else if(cmd=="LSLS"){
                lstext=data;
            }
        }
    }
  }

}


void StartServer(){
  // ===== WebServer ==== //
  MDNS.addService("http","tcp",80);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
      String body="<h1>M5Client</h1>";

      body+="<form method='post' name='fn'>";
      body+="<button onclick=\"fn.action='/sdls';fn.submit()\">ls tap2</button><br>";
      body+="<button onclick=\"fn.action='/restart';fn.submit()\">restart</button><br>";
      body+="</form><hr>";

      body+="<form method='post' name='fq'>";
      body+="<textarea rows='20' name='q'></textarea><br>";
      body+="<button onclick=\"fq.action='/input';fq.submit()\">input</button><br>";
      body+="</form><hr>";

      body+="<form method='post' name='f'>";
      body+="<input type='text' name='filename'><br>";
      body+="<button onclick=\"f.action='/sdread';f.submit()\">read tap2</button>";
      body+="<button onclick=\"f.action='/sdload';f.submit()\">load exec</button>";
      body+="<button onclick=\"f.action='/sdrm';f.submit()\">rm</button><br>";
      body+="<button onclick=\"f.action='/record';f.submit()\">record</button>";
      body+="<button onclick=\"f.action='/recordend';f.submit()\">record end</button><br>";
      body+="<button onclick=\"f.action='/loop';f.submit()\">loop</button>";
      body+="<button onclick=\"f.action='/loopend';f.submit()\">loop end</button><br>";

      body+="<input type='text' name='times'><br>";
      body+="<button onclick=\"f.action='/repeat';f.submit()\">repeat</button>";
      body+="<button onclick=\"f.action='/loopend';f.submit()\">repeat end</button><br>";

      body+="<textarea rows='20' name='text'></textarea><br>";
      body+="<button onclick=\"f.action='/sdsave';f.submit()\">save</button><br>";

      body+="</form><hr>";

      body+="<form method='post' name='fc'>";
      body+="<input type='text' name='cmd'><br>";
      body+="<input type='text' name='filename'><br>";
      body+="<textarea rows='20' name='data'></textarea><br>";
      body+="<button onclick=\"fc.action='/cmd';fc.submit()\">cmd</button><br>";
      body+="</form><hr>";

      AsyncWebServerResponse *response = request->beginResponse(200, "text/html", makePage("M5Client",body));
      request->send(response);
  });

  server.on("/cmd",HTTP_POST,[](AsyncWebServerRequest *request){
    String cmd=request->arg("cmd");
    String filename=request->arg("filename");
    String data=request->arg("data");
    sendCmd(cmd,filename,data);
    request->send(200, "text/plain", q);
  });

  server.on("/cmd",HTTP_GET,[](AsyncWebServerRequest *request){
    String cmd=request->arg("cmd");
    String filename=request->arg("filename");
    String data=request->arg("data");
    sendCmd(cmd,filename,data);
    request->send(200, "text/plain", q);
  });

  server.on("/input",HTTP_POST,[](AsyncWebServerRequest *request){
    String q=request->arg("q");
    Serial.print(q);
    request->send(200, "text/plain", q);
  });

  server.on("/input",HTTP_GET,[](AsyncWebServerRequest *request){
    String q=request->arg("q");
    Serial.print(q);
    request->send(200, "text/plain", q);
  });

  server.on("/sdsave",HTTP_POST,[](AsyncWebServerRequest *request){
    String filename=request->arg("filename");
    String text=request->arg("text");
    sendCmd("SAVE",filename,text);
    request->send(200, "text/plain", filename+"/"+text);
  });

  server.on("/sdload",HTTP_POST,[](AsyncWebServerRequest *request){
    String filename=request->arg("filename");
    sendCmd("LOAD",filename,"");
    request->send(200, "text/plain", filename);
  });

  server.on("/sdrm",HTTP_POST,[](AsyncWebServerRequest *request){
    String filename=request->arg("filename");
    sendCmd("RMRM",filename,"");
    request->send(200, "text/plain", filename);
  });

  server.on("/record",HTTP_POST,[](AsyncWebServerRequest *request){
    String filename=request->arg("filename");
    sendCmd("RECD",filename,"");
    request->send(200, "text/plain", filename);
  });

  server.on("/recordend",HTTP_POST,[](AsyncWebServerRequest *request){
    sendCmd("REND","","");
    request->send(200, "text/plain", "recordend");
  });

  server.on("/repeat",HTTP_POST,[](AsyncWebServerRequest *request){
    String filename=request->arg("filename");
    String times=request->arg("times");
    sendCmd("REPT",filename,times);
    request->send(200, "text/plain", filename+"/"+times);
  });

  server.on("/loop",HTTP_POST,[](AsyncWebServerRequest *request){
    String filename=request->arg("filename");
    sendCmd("LOOP",filename,"");
    request->send(200, "text/plain", filename);
  });

  server.on("/loopend",HTTP_POST,[](AsyncWebServerRequest *request){
    sendCmd("LEND","","");
    request->send(200, "text/plain", "loopend");
  });

  server.on("/sdread",HTTP_POST,[](AsyncWebServerRequest *request){
    String filename=request->arg("filename");
    Serial.println("^B$ READ "+filename+"/");
    request->send(200, "text/plain", readtext);
  });

  server.on("/sdls",HTTP_POST,[](AsyncWebServerRequest *request){
    Serial.println("^B$ LSLS ");
    request->send(200, "text/plain", lstext);
  });

  server.onNotFound([](AsyncWebServerRequest *request){
      AsyncWebServerResponse *response = request->beginResponse(200, "text/html", makePage("404","<h1>404 Not Found</h1>"));
      request->send(response);
  });

  server.on("/restart", HTTP_POST, [](AsyncWebServerRequest *request) {
      request->send(200, "text/plain", "true");
      shouldReboot = true;
  });

  server.begin();
  
  if(debug) sendCmd("CMSG","8266","server started");
}

String makePage(String title, String contents) {
  String s = "<!DOCTYPE html><html><head>";
  s += "<meta name=\"viewport\" content=\"width=device-width,user-scalable=0\">";
  s += "<title>";
  s += title;
  s += "</title></head><body>";
  s += contents;
  s += "</body></html>";
  return s;
}

void jumpPath(AsyncWebServerRequest *request,String path){
  AsyncWebServerResponse *response = request->beginResponse(302, "text/plain", "");
  response->addHeader("Location",path);
  request->send(response);
}

void sendCmd(String cmd,String name,String data){
    Serial.print("\001"+cmd+"\002"+name+"\003"+data+"\004");
}

String parseCmd(String s,char s,char e){
    return s.substring(s.indexOf(s),s.indexOf(e));
}
