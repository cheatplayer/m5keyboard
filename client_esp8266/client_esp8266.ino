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
const char *ssid = "TP-LINK_M5CL";
const char *password = "cheatplayer"; //min 8 chars
/* ============= ======================= ============= */

String lstext="";
String readtext="";

AsyncWebServer server(80);
FSInfo fs_info;

bool shouldReboot = false;

//Script stuff
bool runLine = false;
bool runScript = false;
File script;

uint8_t scriptBuffer[bufferSize];
uint8_t scriptLineBuffer[bufferSize];
int bc = 0; //buffer counter
int lc = 0; //line buffer counter

void setup() {
  
  Serial.begin(BAUD_RATE);
  delay(2000);
  if(debug) Serial.println("\nstarting...\nSSID: "+ (String)ssid +"\nPassword: "+ (String)password);

  EEPROM.begin(4096);
  SPIFFS.begin();
  
  WiFi.mode(WIFI_STA);
  WiFi.softAP(ssid, password,1, false);
  
  // ===== WebServer ==== //
  MDNS.addService("http","tcp",80);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
      String body="<h1>M5Client</h1>";
      body+="<form method='post' name='fn'>";
      body+="<button onclick=\"fn.action='/sdls';fn.submit()\">ls</button><br>";
      body+="<button onclick=\"fn.action='/restart';fn.submit()\">restart</button><br>";
      body+="</form>";
      body+="<form method='post' name='f'>";
      body+="<input type='text' name='filename'><br>";
      body+="<button onclick=\"f.action='/sdread';f.submit()\">read</button>";
      body+="<button onclick=\"f.action='/sdload';f.submit()\">load</button>";
      body+="<button onclick=\"f.action='/sdrm';f.submit()\">rm</button><br>";
      body+="<button onclick=\"f.action='/record';f.submit()\">record</button>";
      body+="<button onclick=\"f.action='/recordend';f.submit()\">record end</button><br>";
      body+="<button onclick=\"f.action='/loop';f.submit()\">loop</button>";
      body+="<button onclick=\"f.action='/loopend';f.submit()\">loop end</button><br>";
      body+="<textarea rows='20' name='text'></textarea><br>";
      body+="<button onclick=\"f.action='/sdsave';f.submit()\">save</button><br>";
      body+="<input type='text' name='times'><br>";
      body+="<button onclick=\"f.action='/repeat';f.submit()\">repeat</button><br>";
      body+="</form>";
      body+="<form method='post' name='fq'>";
      body+="<textarea rows='20' name='q'></textarea><br>";
      body+="<button onclick=\"fq.action='/cmd';fq.submit()\">cmd</button>";
      body+="<button onclick=\"fq.action='/input';fq.submit()\">input</button>";
      body+="</form>";
	  AsyncWebServerResponse *response = request->beginResponse(200, "text/html", makePage("M5Client",body));
	  request->send(response);
  });

  server.on("/cmd",HTTP_POST,[](AsyncWebServerRequest *request){
    String q=request->arg("q");
    Serial.print("^B$ "+q);
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
    Serial.println("^B$ SAVE "+filename+"/"+text);
    request->send(200, "text/plain", filename+"/"+text);
  });

  server.on("/sdload",HTTP_POST,[](AsyncWebServerRequest *request){
    String filename=request->arg("filename");
    Serial.println("^B$ LOAD "+filename+"/");
    request->send(200, "text/plain", filename);
  });

  server.on("/sdrm",HTTP_POST,[](AsyncWebServerRequest *request){
    String filename=request->arg("filename");
    Serial.println("^B$ RMRM "+filename+"/");
    request->send(200, "text/plain", filename);
  });

  server.on("/record",HTTP_POST,[](AsyncWebServerRequest *request){
    String filename=request->arg("filename");
    Serial.println("^B$ RECD "+filename+"/");
    request->send(200, "text/plain", filename);
  });

  server.on("/recordend",HTTP_GET,[](AsyncWebServerRequest *request){
    Serial.println("^B$ REND ");
    request->send(200, "text/plain", "recordend");
  });

  server.on("/repeat",HTTP_POST,[](AsyncWebServerRequest *request){
    String filename=request->arg("filename");
    String times=request->arg("times");
    Serial.println("^B$ REPT "+filename+"/"+times+"/");
    request->send(200, "text/plain", filename+"/"+times);
  });

  server.on("/loop",HTTP_POST,[](AsyncWebServerRequest *request){
    String filename=request->arg("filename");
    Serial.println("^B$ LOOP "+filename+"/");
    request->send(200, "text/plain", filename);
  });

  server.on("/loopend",HTTP_POST,[](AsyncWebServerRequest *request){
    String filename=request->arg("filename");
    Serial.println("^B$ LEND "+filename+"/");
    request->send(200, "text/plain", filename);
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

  server.on("/restart", HTTP_GET, [](AsyncWebServerRequest *request) {
      request->send(200, "text/plain", "true");
	  shouldReboot = true;
  });


  server.begin();
  
  if(debug) Serial.println("started");
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

void sendBuffer(){
  for(int i=0;i<bc;i++) Serial.write((char)scriptBuffer[i]);
  runLine = false;
  bc = 0;
}

void addToBuffer(){
  if(bc + lc > bufferSize) sendBuffer();
  for(int i=0;i<lc;i++){
    scriptBuffer[bc] = scriptLineBuffer[i];
    bc++;
  }
  lc = 0;
}

void loop() {
  if(shouldReboot) ESP.restart();
  
  if(Serial.available()) {
    uint8_t answer = Serial.read();
    if(answer == 0x99) {
      if(debug) Serial.println("done");
      runLine = true;
	}
	else {
		String originStr = (char)answer + Serial.readStringUntil(255);
        String tag=originStr.substring(0,3);
		if( tag== "^B$") {
            String cmd=originStr.substring(4,8);
            String query=originStr.substring(9);
            if(cmd=="REST"){
                settings.reset();
                shouldReboot = true;
            }else if(cmd=="READ"){
                readtext=query;
            }else if(cmd=="LSLS"){
                lstext=query;
            }
		}
	}
  }

  if(runScript && runLine){
    if(script.available()){
      uint8_t nextChar = script.read();
	  if(debug) Serial.write(nextChar);
      scriptLineBuffer[lc] = nextChar;
      lc++;
      if(nextChar == 0x0D || lc == bufferSize) addToBuffer();
    }else{
      addToBuffer();
      if(bc > 0) sendBuffer();
      runScript = false;
      script.close();
    }
  }

}
