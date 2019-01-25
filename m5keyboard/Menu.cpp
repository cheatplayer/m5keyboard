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

std::string record_str="";
void Menu::record(char key_val){
    // int i=(int)key_val;//todo record special key
    record_str+=std::string(1,key_val);
}

void Menu::clear(){
    Display::clear();
    M5.Lcd.setCursor(0,0);
    record_str="";
}

std::string savename="";
std::string savemsg="";
void Menu::save(){
    if(savename==""){
        savemsg=record_str;
        Display::info("input file name");
        savename="/";
    }else{
        savename+=record_str;
        if(SDCard::write(savename.c_str(),savemsg.c_str())){
            Display::info("save ok");
        }else{
            Display::info("save fail");
        }
        savename="";
        savemsg="";
    }
    Menu::clear();    
}

std::string readname="";
std::vector<std::string> readfiles;
void Menu::read(){
    if(readname==""){
        readfiles=SDCard::ls("/");
        readname=readfiles[0];
        Display::info(readname.c_str());
    }
}
