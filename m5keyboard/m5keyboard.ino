
/*
 2019/1/11 by cp
 */

#include <M5Stack.h>
#include "BLEHIDKeyboard.h"
#include "SDCard.h"
#include "Menu.h"
#include "Display.h"

#define KEYBOARD_I2C_ADDR     0X08
#define KEYBOARD_INT          5

extern void StartBLEServer();
extern void StopBLEServer();
extern void inputKeyValue(int key_val);

int menuindex= 0;
const int MENULEN= 6;

const char* menuname[MENULEN]={
    "BLE",
    "halt",
    "save",
    "load",
    "hack",
    "rm"
};

void (*funcarr[MENULEN])()={
  StartBLEServer,
  Menu::halt,
  Menu::save,
  Menu::read,
  Menu::hack,
  Menu::rm
};

void (*funcarrNext[MENULEN])()={
    StopBLEServer,
    Menu::clear,
    Menu::clear,
    Menu::readNext,
    Menu::hackStop,
    Menu::rmNext
};

void setup() {
  M5.begin();              
  Serial.begin(115200);         
  Wire.begin();

  Display::init();
  Display::menu(menuname[menuindex]);
  SDCard::mount();

  pinMode(KEYBOARD_INT, INPUT_PULLUP);  //m5stack face keyboard
}

void loop() {
  if(M5.BtnA.wasPressed()) {
    menuindex++;
    if(menuindex >= MENULEN){
        menuindex=0;
    }
    Display::menu(menuname[menuindex]);
  }

  if(M5.BtnB.wasPressed()) {
    funcarrNext[menuindex]();
  }
  
  if (M5.BtnC.wasPressed()) {
    funcarr[menuindex]();
  }

  if(digitalRead(KEYBOARD_INT) == LOW) {
    Wire.requestFrom(KEYBOARD_I2C_ADDR, 1);  // request 1 byte from keyboard
    while (Wire.available()) { 
      uint8_t key_val = Wire.read();      
      if(key_val != 0) {
        Display::print((char)key_val);
        Menu::record((char)key_val);
        inputKeyValue((int)key_val);
      }
    }
  }
  M5.update();
}
