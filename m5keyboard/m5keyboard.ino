
/*
 2019/1/11 by cp
 */

#include <M5Stack.h>
#include "BLEHIDKeyboard.h"

#define KEYBOARD_I2C_ADDR     0X08
#define KEYBOARD_INT          5

BLEServerTask* bleservertask = NULL;

void StartBLEServer(void)
{
  bleservertask = new BLEServerTask();
  bleservertask->setStackSize(20000);
  bleservertask->start();
}

void setup() {
  M5.begin();                   // M5STACK INITIALIZE
  M5.Lcd.setTextFont(2);
  M5.Lcd.fillScreen(BLACK);     // CLEAR SCREEN
  M5.Lcd.setTextColor(WHITE);
  Serial.begin(115200);         // SERIAL

  StartBLEServer();

  Wire.begin();
  pinMode(KEYBOARD_INT, INPUT_PULLUP);  //m5stack face keyboard
}

void loop() {
  if(M5.BtnA.wasPressed()) {
    InputTask *task = new InputTask("Hello From M5Stack!\n");
    task->start();
    InputTask::inputtasks.push_back(task);
  }

  if(M5.BtnB.wasPressed()) {
    KEYMAP payload[1];
    payload[0] = {0x4c, KEY_CTRL | KEY_ALT}; // CTRL+ALT+DEL !!!!
    InputTask *task = new InputTask(payload, 1);
    task->start();
  }
  
  if (M5.BtnC.wasPressed()) {
    Serial.println("btn c");
  }

  if(digitalRead(KEYBOARD_INT) == LOW) {
    Wire.requestFrom(KEYBOARD_I2C_ADDR, 1);  // request 1 byte from keyboard
    while (Wire.available()) { 
      uint8_t key_val = Wire.read();                  // receive a byte as character
      if(key_val != 0) {
          Serial.print((char)key_val);
          Serial.print((int)key_val);
          Serial.printf("0x%02X ",key_val);
          M5.Lcd.print((char)key_val);
          if(isConnected){
            KEYMAP payload[1];
            payload[0]=m5keymap[(int)key_val];
            InputTask *task = new InputTask(payload,1);
            task->start();
          }
      }
    }
  }
  M5.update();
}
