/*
    2019/1/23 by cp
*/
#include <M5Stack.h>
#include "Menu.h"
#include "Display.h"
#include "SDCard.h"
#include "BLEHIDKeyboard.h"
#include "ShExec.h"
#include "M5Server.h"
#include "M5Client.h"

extern bool isConnected;
extern void inputKeyValue(int key_val);
extern bool isClientConnected;
String clientcmd="";
bool isclientcmd=false;

void Menu::halt(){
    M5.powerOFF();
}

String record_str="";

void Menu::record(char key_val){
    if(isclientcmd){
        clientcmd+=key_val;
    }else{
        int k=(int)key_val;
        inputKeyValue(k);
        String sh=Sh::stringify(key_val);
        record_str+=sh;
        TheClient::sendClient(sh);
        Display::clear();
        std::vector<String> record_vec=Sh::split(record_str,'\r');
        for(int i=0;i<record_vec.size();i=i+1){
            if(record_vec.size()-i<13){
                M5.Lcd.setCursor(5,(13-record_vec.size()+i)*18-18);
                M5.Lcd.print(record_vec[i].c_str());
            }
        }
    }

}

void Menu::clientCmd(){
    clientcmd="";
    isclientcmd=true;
}

void Menu::sendClientCmd(){
    TheClient::sendCmd(clientcmd);
    clientcmd="";
    isclientcmd=false;
}

void Menu::clear(){
    Display::clear();
    record_str="";
}

String savename="";
String savemsg="";
void Menu::save(){
    if(savename==""){
        savemsg=record_str;
        Display::info("input file name");
        savename="/";
        isclientcmd=true;
    }else{
        savename+=clientcmd;
        if(SDCard::write(savename.c_str(),savemsg.c_str())){
            Display::result("save ok");
        }else{
            Display::result("save fail");
        }
        isclientcmd=false;
        savename="";
        savemsg="";
    }
    Menu::clear();  
    Menu::rels();  
}

String findname="";
std::vector<String> findfiles;
int findindex=0;
void Menu::find(){
    if(findindex==0){
        Menu::ls();
    }
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
        record_str=String(SDCard::read(findname.c_str()).c_str());
//        M5.Lcd.print(record_str.c_str());
        
        std::vector<String> vec=Sh::split(record_str,'\r');
        for(int i=0;i<vec.size();i=i+1){
            if(i<11){
                M5.Lcd.setCursor(5,i*18+2);
                M5.Lcd.print(vec[i].c_str());
            }
        }
        Display::result("load ok");
    }
}

Exec *runscripttask;
void Menu::runScript(){
  if(isConnected){
    runscripttask= new Exec(record_str);
    runscripttask->start();
    Display::result("runing script...");
  }else if(isClientConnected){
    TheClient::sendClient(record_str);
  }else{
    Display::result("run script fail");  
  }
}

void Menu::runScriptStop(){
    runscripttask->stop();
    delete runscripttask;
    Display::result("run script stoped");
}

bool isRuning=false;
void Menu::runMenu(){
    if(isRuning){
        Menu::runScriptStop();
        isRuning=false;
        Display::menu("run");
    }else{
        Menu::runScript();
        isRuning=true;
        Display::menu("cancel");
    }

}

void Menu::loop(){
  if(isConnected){
    runscripttask= new Exec(record_str);
    runscripttask->isloop=true;
    runscripttask->start();
    Display::result("loop script...");
  }else if(isClientConnected){
    TheClient::sendCmd("SAVE m5loop/"+record_str);
    delay(1000);
    TheClient::sendCmd("LOOP m5loop/");
  }else{
    Display::result("loop script fail");  
  }
}

void Menu::loopStop(){
    if(isConnected){
        runscripttask->isloop=false;
        runscripttask->stop();
        delete runscripttask;
    }else if(isClientConnected){
        TheClient::sendCmd("LEND ");
    }
    Display::result("loop script stoped");
}

bool isLooping=false;
void Menu::loopMenu(){
    if(isLooping){
        Menu::loopStop();
        isLooping=false;
        Display::info("loop");
    }else{
        Menu::loop();
        isLooping=true;
        Display::info("cancel");
    }
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
    TheClient::StartHTTPClient("M5CL","cheatplayer");
}

extern bool isClientConnected;
void Menu::stopClientMenu(){
    M5.Lcd.fillCircle(10,230,3,RED);
    isClientConnected=false;
}

extern void StopBLEServer();
void Menu::stopAll(){
    M5Server::stopAP();
    M5Server::stopSTA();
    Menu::stopClientMenu();
    StopBLEServer();
}
