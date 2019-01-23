
/*
 2019/1/11 by cp
 */

#include <M5Stack.h>
#include "BLEHIDKeyboard.h"
#include "SDCard.h"
#include "Util.h"
#include "Display.h"

#define KEYBOARD_I2C_ADDR     0X08
#define KEYBOARD_INT          5

extern bool isConnected;
extern void StartBLEServer();
extern void inputKeyValue(int key_val);

int menuindex= 0;
int menulen= 3;

const char* menuname[3]={
    "start BLE",
    "mount SD",
    "halt"
};

void (*funcarr[3])()={
  StartBLEServer,
  SDCard::mount,
  Util::halt
  };

void setup() {
  M5.begin();                   // M5STACK INITIALIZE
  Serial.begin(115200);         // SERIAL
  Wire.begin();

  Display::init();
  Display::menu(menuname[menuindex]);

  pinMode(KEYBOARD_INT, INPUT_PULLUP);  //m5stack face keyboard
}

void loop() {
  if(M5.BtnA.wasPressed()) {
    Serial.println(menuname[menuindex]);
    funcarr[menuindex]();
  }

  if(M5.BtnB.wasPressed()) {
    menuindex++;
    if(menuindex >= menulen){
        menuindex=0;
    }
    Display::menu(menuname[menuindex]);
  }
  
  if (M5.BtnC.wasPressed()) {
    Serial.println("btn c");
    KEYMAP payload[1];
    payload[0] = {0x2c, KEY_CTRL | KEY_ALT};
    Serial.println(payload[0].usage);
    Serial.println(payload[0].modifier);
    InputTask *task = new InputTask(payload, 1);
    task->start();
  }

  if(digitalRead(KEYBOARD_INT) == LOW) {
    Wire.requestFrom(KEYBOARD_I2C_ADDR, 1);  // request 1 byte from keyboard
    while (Wire.available()) { 
      uint8_t key_val = Wire.read();         // receive a byte as character
      Serial.print((char)key_val);
      Serial.print((int)key_val);
      Serial.printf("0x%02X ",key_val);
      if(key_val != 0) {
          M5.Lcd.print((char)key_val);
          inputKeyValue((int)key_val);
      }
    }
  }
  M5.update();
}
