/*
    2019/1/23 by cp
*/
#include <M5Stack.h>
#include "Menu.h"
#include <string>
#include "Display.h"
#include "SDCard.h"
#include "BLEHIDKeyboard.h"

extern bool isConnected;

void Menu::halt(){
    M5.powerOFF();
}

std::string record_str="";
void Menu::record(char key_val){
    int i=(int)key_val;
    if(i==8){//backspace
        record_str+="\b";
        return;
    }
    if(i==13){//enter
        record_str+="\n";
        return;
    }
    record_str+=std::string(1,key_val);
}

void Menu::clear(){
    Display::clear();
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
int nowreadindex=0;
void Menu::read(){
    if(readname==""){
        readfiles=SDCard::ls("/");
        readname=readfiles[0];
        Display::info(readname.c_str());
    }else{
        Menu::clear();
        record_str=SDCard::read(readname.c_str());
        M5.Lcd.print(record_str.c_str());
        readname="";
        nowreadindex=0;
        readfiles.clear();
    }
}

void Menu::readNext(){
    nowreadindex++;
    if(nowreadindex>=readfiles.size()){
        nowreadindex=0;
    }
    readname=readfiles[nowreadindex];
    Display::info(readname.c_str());
}

std::string rmname="";
std::vector<std::string> rmfiles;
int nowrmindex=0;
void Menu::rm(){
    if(rmname==""){
        rmfiles=SDCard::ls("/");
        rmname=rmfiles[0];
        Display::info(rmname.c_str());
    }else{
        if(SDCard::rm(rmname.c_str())){
            Display::info("rm ok");
        }else{
            Display::info("rm fail");
        }
        rmfiles.clear();
        rmname="";
        nowrmindex=0;
    }
}

void Menu::rmNext(){
    nowrmindex++;
    if(nowrmindex>=rmfiles.size()){
        nowrmindex=0;
    }
    rmname=rmfiles[nowrmindex];
    Display::info(rmname.c_str());
}

InputTask *hacktask;
void Menu::hack(){
  if(isConnected){
    hacktask= new InputTask(record_str.c_str());
    hacktask->start();
    Display::info("hacking...");
  }else{
    Display::info("hack fail");  
  }
}

void Menu::hackStop(){
    hacktask->stop();
    delete hacktask;
    Display::info("hack stoped");
}
