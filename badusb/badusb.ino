/*
    2019/1/31 by cp
*/

#include "Keyboard.h"
#include <SPI.h>
#include <SD.h>
#include <ArduinoSTL.h>

std::string DEFAULT_FILE_NAME = "DS_Store";

std::vector<std::string> split(std::string str,char sep){
    std::vector<std::string> result;
    std::string::size_type pos1, pos2;
    pos2 = str.find(sep);
    pos1 = 0;
    while (std::string::npos != pos2)
    {
        result.push_back(str.substr(pos1, pos2 - pos1));
        pos1 = pos2 + 1;
        pos2 = str.find(sep, pos1);
    }
    result.push_back(str.substr(pos1));
    return result;
}

int parse(std::string n){
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
    return (int)n[0];
}

void execLine(std::string line){
    if(line.substr(0,3)==">>>"){
        std::vector<std::string> sp=split(line.substr(4),' ');
        int i=0;
        while(i<sp.size()){
            int key=parse(sp[i]);
            Keyboard.press(key);
            i++;
        }
        Keyboard.releaseAll();
   }else if(line.substr(0,3)=="$$$"){
        int n=atoi(line.substr(4).c_str());
        delay(n);
   }else if(line.substr(0,3)==":::"){
        int key=parse(line.substr(4));
        Keyboard.press(key);
        Keyboard.release(key);
   }else{
        Keyboard.write(line.c_str(),line.size());
        Keyboard.releaseAll();
   }
}

void setup(){
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    if(!SD.begin(4)){
        digitalWrite(LED_BUILTIN,LOW);
    }
    File f=SD.open(DEFAULT_FILE_NAME.c_str());
    if(f){
        Keyboard.begin();
        std::string line="";
        while(f.available()){
            char k=f.read();
            if(k=='\r'){
                execLine(line);
                line="";
            }else{
                line+=k;
            }
        }
        execLine(line);
        f.close();
        Keyboard.end();
    }
    digitalWrite(LED_BUILTIN,LOW);
}

void loop(){

}
