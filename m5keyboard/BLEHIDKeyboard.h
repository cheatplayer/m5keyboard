/*
    2019/1/21 by cp
*/

#include "Task.h"

#ifndef BLE_HID_KEYBOARD_H
#define BLE_HID_KEYBOARD_H

#include "BLEDevice.h"
#include "BLEServer.h"
#include "BLEUtils.h"
#include "BLE2902.h"
#include "BLEHIDDevice.h"
#include "HIDKeyboardTypes.h"
#include "M5KeyboardType.h"


class OutputCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic* me);
};

class ServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer* bleserver);
    void onDisconnect(BLEServer* bleserver);
};

class InputTask:public Task{
private:
    KEYMAP payload[256];
    int length = 0;
  
public:
    InputTask(const char *text);
    InputTask(const KEYMAP *payload, int length);
    InputTask();
    void setString(const char *text);
    void setKeys(const KEYMAP *payload, int length);
    void run(void*);
};

class BLEServerTask:public Task {
public:
    void run(void *data);
};

#endif
