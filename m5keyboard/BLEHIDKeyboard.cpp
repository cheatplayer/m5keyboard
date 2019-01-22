/*
    2019/1/21 by cp
*/
#include <vector>
#include <M5Stack.h>
#include "BLEHIDKeyboard.h"

BLEHIDDevice *hid;
BLECharacteristic *input;
BLECharacteristic *output;
bool isConnected = false;
BLEServer *bleserver;
std::vector<InputTask *> inputtasks;

const uint8_t reportMap[] = {
  USAGE_PAGE(1),      0x01,       // Generic Desktop Ctrls
  USAGE(1),           0x06,       // Keyboard
  COLLECTION(1),      0x01,       // Application
  REPORT_ID(1),   0x01,   // REPORTID
  USAGE_PAGE(1),      0x07,       //   Kbrd/Keypad
  USAGE_MINIMUM(1),   0xE0,
  USAGE_MAXIMUM(1),   0xE7,
  LOGICAL_MINIMUM(1), 0x00,
  LOGICAL_MAXIMUM(1), 0x01,
  REPORT_SIZE(1),     0x01,       //   1 byte (Modifier)
  REPORT_COUNT(1),    0x08,
  HIDINPUT(1),           0x02,       //   Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position
  REPORT_COUNT(1),    0x01,       //   1 byte (Reserved)
  REPORT_SIZE(1),     0x08,
  HIDINPUT(1),           0x01,       //   Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position
  REPORT_COUNT(1),    0x05,       //   5 bits (Num lock, Caps lock, Scroll lock, Compose, Kana)
  REPORT_SIZE(1),     0x01,
  USAGE_PAGE(1),      0x08,       //   LEDs
  USAGE_MINIMUM(1),   0x01,       //   Num Lock
  USAGE_MAXIMUM(1),   0x05,       //   Kana
  HIDOUTPUT(1),          0x02,       //   Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile
  REPORT_COUNT(1),    0x01,       //   3 bits (Padding)
  REPORT_SIZE(1),     0x03,
  HIDOUTPUT(1),          0x01,       //   Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile
  REPORT_COUNT(1),    0x06,       //   6 bytes (Keys)
  REPORT_SIZE(1),     0x08,
  LOGICAL_MINIMUM(1), 0x00,
  LOGICAL_MAXIMUM(1), 0x65,       //   101 keys
  USAGE_PAGE(1),      0x07,       //   Kbrd/Keypad
  USAGE_MINIMUM(1),   0x00,
  USAGE_MAXIMUM(1),   0x65,
  HIDINPUT(1),           0x00,       //   Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position
  END_COLLECTION(0)
};

void displayBLEServerStatus()
{
  if(isConnected){
    M5.Lcd.fillCircle(5,230,3,GREEN);
  }else{
    M5.Lcd.fillCircle(5,230,3,RED);
  }
}

void BLEServerTask::run(void *data) {

    BLEDevice::init("M5 BlueTooth Keyboard");
    bleserver = BLEDevice::createServer();
    bleserver->setCallbacks(new ServerCallbacks());

    /*
     * https://www.bluetooth.com/specifications/gatt/viewer
     */   
    hid = new BLEHIDDevice(bleserver);

    input = hid->inputReport(1); // <-- input REPORTID from report map
    output = hid->outputReport(1); // <-- output REPORTID from report map

    output->setCallbacks(new OutputCallbacks());
    std::string name = "cheatplayer";
    hid->manufacturer()->setValue(name);
    hid->pnp(0x02, 0xe502, 0xa111, 0x0210);
    hid->hidInfo(0x00,0x01);
    hid->reportMap((uint8_t*)reportMap, sizeof(reportMap));
    hid->setBatteryLevel(0x01);


    hid->startServices();

    /*
     * Its good to setup advertising by providing appearance and advertised service. This will let clients find our device by type
     */
    BLEAdvertising *pAdvertising = bleserver->getAdvertising();
    pAdvertising->setAppearance(HID_KEYBOARD);
    pAdvertising->addServiceUUID(hid->hidService()->getUUID());
    pAdvertising->start();

    BLESecurity *pSecurity = new BLESecurity();
    pSecurity->setAuthenticationMode(ESP_LE_AUTH_BOND);//a

    Serial.println("server advertising");
    displayBLEServerStatus();
    Serial.println(hid->hidService()->getUUID().toString().c_str());

    delay(1000000);
}



void ServerCallbacks::onConnect(BLEServer* bleserver){
     BLE2902* desc;
     desc = (BLE2902*) input->getDescriptorByUUID(BLEUUID((uint16_t)0x2902));
     desc->setNotifications(true);
     isConnected = true;
     Serial.println("on connect");
     displayBLEServerStatus();
  }

void ServerCallbacks::onDisconnect(BLEServer* bleserver){
    isConnected = false;
    Serial.println("on disconnect");
    displayBLEServerStatus();

    for(int i=0 ; i<inputtasks.size(); i++) {
      InputTask *task = inputtasks[i];
      task->stop();
    }
    inputtasks.clear();
}

/*
 * This callback is connect with output report. In keyboard output report report special keys changes, like CAPSLOCK, NUMLOCK
 * We can add digital pins with LED to show status
 * bit 1 - NUM LOCK
 * bit 2 - CAPS LOCK
 * bit 3 - SCROLL LOCK
 */
void OutputCallbacks::onWrite(BLECharacteristic* me){
    Serial.println("on output");
    const char* value = me->getValue().c_str();
    Serial.println(value);
    displayBLEServerStatus();
}


InputTask::InputTask(const char *text) {
    setString(text);
}
  
InputTask::InputTask(const KEYMAP *payload, int length) {
    setKeys(payload, length);
}

InputTask::InputTask() {
    length = 0;
}

void InputTask::setString(const char *text) {
    length = 0;
    const char *pointer = text;
    while(*pointer){
        KEYMAP map = keymap[(uint8_t)*pointer];
        payload[length++] = map;
        pointer++;
    }
}
  
void InputTask::setKeys(const KEYMAP *payload, int length) {
    int realLen = min(256, length);
    for (int i=0 ; i<realLen ; i++) {
        this->payload[i] = payload[i];
    }
    this->length = realLen;
}
  
void InputTask::run(void*){
    for(int i=0 ; i<length ; i++) {
        KEYMAP map = payload[i];
        /*
         * simulate keydown, we can send up to 6 keys
         */
        uint8_t a[] = {map.modifier, 0x0, map.usage, 0x0,0x0,0x0,0x0,0x0};
        input->setValue(a,sizeof(a));
        input->notify();

        /*
         * simulate keyup
         */
        uint8_t v[] = {0x0, 0x0, 0x0, 0x0,0x0,0x0,0x0,0x0};
        input->setValue(v, sizeof(v));
        input->notify();

        vTaskDelay(100/portTICK_PERIOD_MS);
    }
}
