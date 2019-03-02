/*
 2019/1/11 by cp
 */

#include <M5Stack.h>
#include "BLEHIDKeyboard.h"
#include "SDCard.h"
#include "Menu.h"
#include "Display.h"
#include "M5Server.h"
#include "M5Client.h"
#include "WebServer.h"
#include <Preferences.h>

#define KEYBOARD_I2C_ADDR     0X08
#define KEYBOARD_INT          5

extern void StartBLEServer();
extern void StopBLEServer();
extern void inputKeyValue(int key_val);

extern bool isAPStarted;
extern bool isSTAConnected;
extern WebServer webServer;
extern Preferences preferences;

typedef struct {
    const char* name;
    void (*func)();
} MenuFunc;

const int MENULEN= 9;
MenuFunc menufuncA[MENULEN]={
    {"BLE",StartBLEServer},
    {"save",Menu::save},
    {"load",Menu::load},
    {"run",Menu::runScript},
    {"halt",Menu::halt},
    {"rm",Menu::rm},
    {"stopBLE",StopBLEServer},
    {"STA",Menu::startSTAMenu},
    {"AP",Menu::startAPMenu}
};

MenuFunc menufuncB[MENULEN]={
    {"Client",Menu::startClientMenu},
    {"clear",Menu::clear},
    {"find",Menu::find},
    {"cancel",Menu::runScriptStop},
    {"ls",Menu::ls},
    {"find",Menu::find},
    {"stopClient",Menu::stopClientMenu},
    {"stop",M5Server::stopSTA},
    {"stop",M5Server::stopAP}
};

int menuindex= 0;

void setup() {
  M5.begin();              
  Serial.begin(115200);         
  Wire.begin();
  preferences.begin("wifi-config");

  Display::init();
  Display::menu(menufuncA[menuindex].name);
  Display::info(menufuncB[menuindex].name);
  Display::result("menu");
  SDCard::mount();
  Menu::rels();

  pinMode(KEYBOARD_INT, INPUT_PULLUP);  //m5stack face keyboard
}

void loop() {
  if(M5.BtnA.wasPressed()) {
    menufuncA[menuindex].func();
  }

  if(M5.BtnB.wasPressed()) {
    menufuncB[menuindex].func();
  }
  
  if (M5.BtnC.wasPressed()) {
    menuindex++;
    if(menuindex >= MENULEN){
        menuindex=0;
    }
    Display::menu(menufuncA[menuindex].name);
    Display::info(menufuncB[menuindex].name);
    Display::result("menu");
  }

  if(digitalRead(KEYBOARD_INT) == LOW) {
    Wire.requestFrom(KEYBOARD_I2C_ADDR, 1);  // request 1 byte from keyboard
    while (Wire.available()) { 
      uint8_t key_val = Wire.read();      
      if(key_val != 0) {
        Display::print((char)key_val);
        Menu::record((char)key_val);
        inputKeyValue((int)key_val);
        TheClient::sendClient(key_val);
      }
    }
  }
  if(isAPStarted||isSTAConnected){
      webServer.handleClient();
  }
  M5.update();
}
