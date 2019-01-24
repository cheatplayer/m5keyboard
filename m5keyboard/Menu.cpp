/*
    2019/1/23 by cp
*/
#include <M5Stack.h>
#include "Menu.h"
#include <string>
#include "Display.h"
#include "SDCard.h"

void Menu::halt(){
    M5.powerOFF();
}

string record_str="";
void Menu::record(char key_val){
    M5.Lcd.print(key_val); 
    string s=key_val;
    record_str+=s;
    Serial.println(record_str);
}

void Menu::clear(){
    Display::clear();
    M5.Lcd.setCursor(0,0);
    record_str="";
}

string savename="";
string savemsg="";
void Menu::save(){
    if(savename==""){
        savemsg=record_str;
        Display::info("input file name");
        savename="/";
    }else{
        savename+=record_str;
        if(SDCard::write(savename,savemsg)){
            Display::info("save ok");
        }else{
            Display::info("save fail");
        }
        savename="";
        savemsg="";
    }
        Menu::clear();
}

string readname="";
std::vector<string> readfiles;
void Menu::read(){
    if(readname==""){
        readfiles=SDCard::ls('/');
        readname=readfiles[0];
        Display::info(readname);
    }
}