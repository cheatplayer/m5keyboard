/*
    2019/1/22 by cp
*/
#include <M5Stack.h>
#include "SDCard.h"

bool isMounted=false;

void SDCard::displaySDStatus()
{
  if(isMounted){
  }else{
    M5.Lcd.print("SD Card Mount Failed");
  }
}

void SDCard::mount(){
    if(!SD.begin(4)){
        Serial.println("Card Mount Failed");
        isMounted=false;
        SDCard::displaySDStatus();
        return;
    }

    uint8_t cardType = SD.cardType();

    if(cardType == CARD_NONE){
        Serial.println("No SD card attached");
        return;
    }

    Serial.print("SD Card Type: ");
    if(cardType == CARD_MMC){
        Serial.println("MMC");
    } else if(cardType == CARD_SD){
        Serial.println("SDSC");
    } else if(cardType == CARD_SDHC){
        Serial.println("SDHC");
    } else {
        Serial.println("UNKNOWN");
    }

    uint64_t cardSize = SD.cardSize() / (1024 * 1024);
    Serial.printf("SD Card Size: %lluMB\n", cardSize);
    isMounted=true;
    SDCard::displaySDStatus();
}

std::vector<String> SDCard::ls(const char *path){
    std::vector<String> v;
    if(!isMounted){
        return v;
    }
    File root = SD.open(path);
    if(!root||!root.isDirectory()){
        return v;
    }
    File file=root.openNextFile();
    while(file){
        String filename=file.name();
        if(file.isDirectory()){
            // v.push_back(filename+"/");
        }else{
            v.push_back(filename);
        }
        file=root.openNextFile();
    }
    root.close();
    return v;
}

bool SDCard::mkdir(const char *path){
    if(isMounted&&SD.mkdir(path)){
        return true;
    }else{
        return false;
    }
}

bool SDCard::rmdir(const char *path){
    if(isMounted&&SD.rmdir(path)){
        return true;
    }else{
        return false;
    }
}

String SDCard::read(const char *path){
    if(isMounted){
        File file=SD.open(path);
        if(!file){
            return "";
        }
        std::string result;
      while(file.available()){
          result+=file.read();
      }
      file.close();
      return String()+result.c_str();
    }

}

bool SDCard::write(const char *path,const char *message){
    if(!isMounted){
        return false;
    }
    File file = SD.open(path, FILE_WRITE);
    if(!file){
        return false;
    }
    if(file.print(message)){
        file.close();
        return true;
    } else {
        file.close();
        return false;
    }
    
}

bool SDCard::append(const char *path,const char *message){
    if(!isMounted){
        return false;
    }
    File file = SD.open(path, FILE_APPEND);
    if(!file){
        return false;
    }
    if(file.print(message)){
        file.close();
        return true;
    } else {
        file.close();
        return false;
    }
}

bool SDCard::mv(const char *from,const char *to){
    if(isMounted&&SD.rename(from,to)){
        return true;
    }else{
        return false;
    }
}

bool SDCard::rm(const char *path){
    if(isMounted&&SD.remove(path)){
        return true;
    }else{
        return false;
    }
}
