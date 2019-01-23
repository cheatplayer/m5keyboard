/*
    2019/1/22 by cp
*/
#include <M5Stack.h>
#include "SDCard.h"
#include "FS.h"
#include "SD.h"
#include "SPI.h"

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
