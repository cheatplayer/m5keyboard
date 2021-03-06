/*
    2019/2/20 by cp
*/

#include "Keyboard.h"
#include <SPI.h>
#include <SD.h>
#include "SDCard.h"

String DEFAULT_FILE_NAME = "default";
int defaultDelay = 10;

#define ExternSerial Serial1

void setup(){

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN,HIGH);

    Serial.begin(57200);
    ExternSerial.begin(57200);
    Keyboard.begin();

    if(SD.begin(4)){
        digitalWrite(LED_BUILTIN,LOW);
    }

    delay(5000);
    
    bool dfresult=exec(DEFAULT_FILE_NAME.c_str());

}

String originStr = "";
String bufferStr = "";
String last = "";
String recordfilename="";
String repeatfilename="";
int repeattimes=0;
String loopfilename="";

void loop(){
    if(ExternSerial.available()) {
        originStr = ExternSerial.readStringUntil(255);
        Serial.println(originStr); 
        if(originStr[0]=='\001'){
            cmd();
            originStr = "";
        }else{
            bufferStr = originStr;
            if(recordfilename!=""){
                SDCard::append(recordfilename.c_str(),originStr.c_str());
            }
            originStr = "";
        }
    }
  
    if(bufferStr.length() > 0){
        while(bufferStr.length() > 0){
            int latest_return = bufferStr.indexOf("\r");
            if(latest_return == -1){
                Serial.println("run: "+bufferStr);
                execLine(bufferStr);
                bufferStr = "";
            } else{
                Serial.println("run: '"+bufferStr.substring(0, latest_return)+"'");
                execLine(bufferStr.substring(0, latest_return));
                last=bufferStr.substring(0, latest_return);
                bufferStr = bufferStr.substring(latest_return + 1);
            }
        }
        bufferStr = "";
        ExternSerial.write(0x04);
        Serial.println("done");
    }

    if(repeatfilename!="" && repeattimes>0){
        exec(repeatfilename.c_str());
        repeattimes--;
    }

    if(loopfilename!=""){
        exec(loopfilename.c_str());
    }
}

String parseCmd(String s,String b,String e){
    return s.substring(s.indexOf(b)+1,s.indexOf(e));
}

void cmd(){
    // "\001CMD\002NAME\003DATA\004"
    String cmd=parseCmd(originStr,"\001","\002");
    Serial.println(cmd);
    String filename=parseCmd(originStr,"\002","\003");
    Serial.println(filename);
    String data=parseCmd(originStr,"\003","\004");
    if(cmd=="RMRM"){
        bool rmresult=SDCard::rm(filename.c_str());
        if(!rmresult){
            digitalWrite(LED_BUILTIN, HIGH);
        }
    }else if(cmd=="EXEC"||cmd=="LOAD"){
        bool loadresult=exec(filename.c_str());
        if(!loadresult){
            digitalWrite(LED_BUILTIN, HIGH);
        }
    }else if(cmd=="SAVE"){
        bool saveresult=SDCard::write(filename.c_str(),data.c_str());
        if(!saveresult){
            digitalWrite(LED_BUILTIN, HIGH);
        }
    }else if(cmd=="RECD"){
        recordfilename=filename;
    }else if(cmd=="REND"){
        recordfilename="";
    }else if(cmd=="REPT"){
        repeatfilename=filename;
        repeattimes=atoi(data.c_str());          
    }else if(cmd=="LOOP"){
        loopfilename=filename;
    }else if(cmd=="LEND"){
        loopfilename="";
        repeatfilename="";
        repeattimes=0;
    }else if(cmd=="READ"){
        String readtext=SDCard::read(filename.c_str());
        Serial.println(readtext);
        ExternSerial.write(("\001READ\002"+filename+"\003"+readtext.c_str()+"\004").c_str());
    }else if(cmd=="LSLS"){
        String lstext=SDCard::ls(filename.c_str());
        ExternSerial.write(("\001LSLS\002"+filename+"\003"+lstext+"\004").c_str());
    }else if(cmd=="CMSG"){
        Serial.print("msg: ");
        Serial.println(data);
    }
}

bool exec(const char *filename){
    digitalWrite(LED_BUILTIN,HIGH);
    File f=SD.open(filename);
    if(f){
        String line="";
        while(f.available()){
            char k=f.read();
            Serial.print(k);
            if(k=='\r'){
                execLine(line);
                line="";
            }else{
                line+=k;
            }
        }
        execLine(line);
        f.close();
        digitalWrite(LED_BUILTIN,LOW);
        return true;
    }else{
        Serial.println("no default script");
        Serial.println(SDCard::ls("/"));
        return false;
    }
}

/*
String to std::string
    const char *temp = const_cast<char*>(line.c_str());
    std::string t=temp;
*/

void execLine(String line){
    //delay(defaultDelay);
    if(line.substring(0,3)==">>>"){
        String t=line.substring(4);
        while(t.indexOf(" ")!=-1){
            Serial.println(t.substring(0,t.indexOf(" ")));
            int key=parse(t.substring(0,t.indexOf(" ")));
            Keyboard.press(key);
            t=t.substring(t.indexOf(" ")+1);
        }
        Serial.print(t);
        Keyboard.press(parse(t));
        delay(defaultDelay);
        Keyboard.releaseAll();
        delay(defaultDelay);
    }else if(line.substring(0,3)=="$$$"){
        int n=atoi(line.substring(4).c_str());
        delay(n);
    }else if(line.substring(0,3)==":::"){
        int key=parse(line.substring(4));
        Keyboard.press(key);
        delay(defaultDelay);
        Keyboard.release(key);
        delay(defaultDelay);
    }else if(line.length()==1){
        char key=line[0];
        Keyboard.press(key);
        Keyboard.releaseAll();
    }else{
        Keyboard.write(line.c_str(),line.length());
        delay(defaultDelay);
        Keyboard.releaseAll();
        delay(defaultDelay);
    }
}

int parse(String n){
    if(n=="ENTER")return 0xB0;
    if(n=="LCTRL"||n=="CTRL")return 0x80;
    if(n=="LSHIFT"||n=="SHIFT")return 0x81;
    if(n=="LALT"||n=="ALT")return 0x82;
    if(n=="LMETA"||n=="META"||n=="GUI"||n=="CMD")return 0x83;
    if(n=="RCTRL")return 0x84;
    if(n=="RSHIFT")return 0x85;
    if(n=="RALT") return 0x86;
    if(n=="RMETA")return 0x87;
    if(n=="DEL")return 0xD4;
    if(n=="F1")return 0xC2;
    if(n=="F2")return 0xC3;
    if(n=="F3")return 0xC4;
    if(n=="F4")return 0xC5;
    if(n=="F5")return 0xC6;
    if(n=="F6")return 0xC7;
    if(n=="F7")return 0xC8;
    if(n=="F8")return 0xC9;
    if(n=="F9")return 0xCA;
    if(n=="F10")return 0xCB;
    if(n=="F11")return 0xCC;
    if(n=="F12")return 0xCD;
    if(n=="ESC")return 0xB1;
    if(n=="CAPS")return 0xC1;
    if(n=="UP")return 0xDA;
    if(n=="LEFT")return 0xD8;
    if(n=="DOWN")return 0xD9;
    if(n=="RIGHT")return 0xD7;
    if(n=="SPACE")return (int)' ';
    if(n=="TAB")return 0x09;
    return (int)n[0];
}
