/*
    2019/1/22 by cp
*/
#include <M5Stack.h>

void SDCard::displaySDStatus()
{
  if(SDCard::isMounted){
    M5.Lcd.fillCircle(10,230,3,GREEN);
  }else{
    M5.Lcd.fillCircle(10,230,3,RED);
  }
}

void SDCard::mount(){
    if(!SD.begin(4)){
        Serial.println("Card Mount Failed");
        SDCard::isMounted=false;
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
    SDCard::isMounted=true;
    SDCard::displaySDStatus();
}

void SDCard::ls(const char *path){

}

bool SDCard::mkdir(const char *path){
    if(SDCard.isMounted&&SD.mkdir(path)){
        return true;
    }else{
        return false;
    }
}

bool SDCard::rmdir(const char *path){
    if(SDCard.isMounted&&SD.rmdir){
        return true;
    }else{
        return false;
    }
}

std::string SDCard::read(const char *path){
    if(SDCard.isMounted){
        File file=SD.open(path);
        if(!file){
            return "";
        }
    }
    std::string result;
    while(file.available()){
        result+=file.read();
    }
    file.close();
    return result;
}

bool SDCard::write(const char *path,const char *message){
    File file = SD.open(path, FILE_WRITE);
    if(!file||!SDCard.isMounted){
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
    File file = SD.open(path, FILE_APPEND);
    if(!file||!SDCard::isMounted){
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
    if(SDCard::isMounted&&SD.rename(from,to)){
        return true;
    }else{
        return false;
    }
}

bool SDCard::rm(const char *path){
    if(SDCard::isMounted&&SD.remove(path)){
        return true;
    }else{
        return false;
    }
}