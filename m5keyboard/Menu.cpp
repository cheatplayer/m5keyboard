/*
    2019/1/23 by cp
*/
#include <M5Stack.h>
#include "Menu.h"
#include <string>
#include "Display.h"
#include "SDCard.h"
#include "BLEHIDKeyboard.h"
#include "ShExec.h"
#include "M5Server.h"
#include "M5Client.h"

extern bool isConnected;

void Menu::halt(){
    M5.powerOFF();
}

std::string record_str="";
void Menu::record(char key_val){
    int i=(int)key_val;
    std::string sh=Sh::stringify(key_val);
    record_str+=sh;
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
            Display::result("save ok");
        }else{
            Display::result("save fail");
        }
        savename="";
        savemsg="";
    }
    Menu::clear();  
    Menu::rels();  
}

std::string findname="";
std::vector<std::string> findfiles;
int findindex=0;
void Menu::find(){
    if(findfiles.size()==0){
        return;
    }
    if(findindex>=findfiles.size()-1){
        findindex=0;
    }else{
        findindex++;
    }
    findname=findfiles[findindex];
    Display::info(findname.c_str());
}

void Menu::rels(){
    findfiles.clear();
    findfiles=SDCard::ls("/");
    findname="";
    findindex=0;

}

void Menu::ls(){
    Menu::rels();
    Menu::clear();
    int i=0;
    while(i<findfiles.size()){
      M5.Lcd.println(findfiles[i].c_str());
      i++;
    } 
}

void Menu::rm(){
    if(findname==""){
        Display::result("find first");
    }else{
        if(SDCard::rm(findname.c_str())){
            Display::result("rm ok");
        }else{
            Display::result("rm fail");
        }
        Menu::rels();
    }
}

void Menu::load(){
    if(findname==""){
        Display::result("find first");
    }else{
        Menu::clear();
        record_str=SDCard::read(findname.c_str());
        M5.Lcd.print(record_str.c_str());
        Display::result("load ok");
    }
}

Exec *runscripttask;
void Menu::runScript(){
  if(isConnected){
    runscripttask= new Exec(record_str);
    runscripttask->start();
    Display::result("runing script...");
  }else{
    Display::result("run script fail");  
  }
}

void Menu::runScriptStop(){
    runscripttask->stop();
    delete runscripttask;
    Display::result("run script stoped");
}

void Menu::startSTAMenu(){
    M5Server::startSTA();
    CheckServerTask *checkservertask= new CheckServerTask();
    checkservertask->start();
    M5Server::startServer();
}

void Menu::startAPMenu(){
    M5Server::scanNetworks();
    M5Server::startAP();
    M5Server::startServer();
}

void Menu::startClientMenu(){
    TheClient::StartHTTPClient("TP-LINK_M5CL","cheatplayer");
}

extern bool isClientConnected;
void Menu::stopClientMenu(){
    M5.Lcd.fillCircle(10,230,3,RED);
    isClientConnected=false;
}
