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
    if(n=="ENTER")return 13;
    if(n=="LCTRL"||n=="CTRL")return 144;
    if(n=="LSHIFT"||n=="SHIFT")return 145;
    if(n=="LALT"||n=="ALT")return 146;
    if(n=="LMETA"||n=="META"||n=="GUI"||n=="CMD")return 147;
    if(n=="RCTRL")return 150;
    if(n=="RSHIFT")return 151;
    if(n=="RALT") return 152;
    if(n=="RMETA")return 153;
    if(n=="DEL")return 127;
    if(n=="F1")return 154;
    if(n=="F2")return 155;
    if(n=="F3")return 156;
    if(n=="F4")return 157;
    if(n=="F5")return 158;
    if(n=="F6")return 159;
    if(n=="F7")return 160;
    if(n=="F8")return 161;
    if(n=="F9")return 164;
    if(n=="F10")return 165;
    if(n=="F11")return 166;
    if(n=="F12")return 167;
    if(n=="ESC")return 175;
    if(n=="CAPS")return 182;
    if(n=="UP")return 183;
    if(n=="LEFT")return 191;
    if(n=="DOWN")return 192;
    if(n=="RIGHT")return 193;
    if(n=="SPACE")return 32;
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
