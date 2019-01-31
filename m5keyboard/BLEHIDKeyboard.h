/*
    2019/1/21 by cp
*/
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
#endif
