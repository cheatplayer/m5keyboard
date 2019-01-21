
/*
 2019/1/11 by cp
 */

#include <M5Stack.h>

#include "BLEDevice.h"
#include "BLEServer.h"
#include "BLEUtils.h"
#include "BLE2902.h"
#include "BLEHIDDevice.h"
#include "HIDKeyboardTypes.h"
#include "M5KeyboardType.h"
#include <string>
#include <vector>
#include "Task.h"

#define KEYBOARD_I2C_ADDR     0X08
#define KEYBOARD_INT          5

static BLEHIDDevice* hid;
BLECharacteristic* input;
BLECharacteristic* output;
bool isConnected = false;

void displayStatus(const char* output)
{
  if(isConnected){
    M5.Lcd.fillCircle(5,230,3,GREEN);
  }else{
    M5.Lcd.fillCircle(5,230,3,RED);
  }
  if(output=="2"){
    M5.Lcd.fillCircle(10,230,3,GREEN);
  }
}

/*
 * This callback is connect with output report. In keyboard output report report special keys changes, like CAPSLOCK, NUMLOCK
 * We can add digital pins with LED to show status
 * bit 1 - NUM LOCK
 * bit 2 - CAPS LOCK
 * bit 3 - SCROLL LOCK
 */
class OutputCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic* me){
    Serial.println("on output");
    const char* value = me->getValue().c_str();
    Serial.println(value);
    displayStatus(value);
  }
};

class InputTask : public Task {
  private:
  KEYMAP payload[256];
  int length = 0;
  
  public:
  InputTask(const char *text) {
    this->setString(text);
  }
  
  InputTask(const KEYMAP *payload, int length) {
    this->setKeys(payload, length);
  }

  InputTask() {
    this->length = 0;
  }

  void setString(const char *text) {
    this->length = 0;
    const char *pointer = text;
    while(*pointer){
        KEYMAP map = keymap[(uint8_t)*pointer];
        this->payload[this->length++] = map;
        pointer++;
    }
  }
  
  void setKeys(const KEYMAP *payload, int length) {
    int realLen = min(256, length);
    for (int i=0 ; i<realLen ; i++) {
      this->payload[i] = payload[i];
    }
    this->length = realLen;
  }

  void halt() {
    vTaskDelete(NULL);
  }
  
  private:
  void run(void*){
      for(int i=0 ; i<this->length ; i++) {
        KEYMAP map = this->payload[i];
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
};

std::vector<InputTask *> tasks;

class ServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* pServer){
     BLE2902* desc;
     desc = (BLE2902*) input->getDescriptorByUUID(BLEUUID((uint16_t)0x2902));
     desc->setNotifications(true);
     isConnected = true;
     Serial.println('on connect');
     displayStatus("C");
  }

  void onDisconnect(BLEServer* pServer){
    isConnected = false;
    Serial.println('on disconnect');
    displayStatus("D");

    for(int i=0 ; i<tasks.size(); i++) {
      InputTask *task = tasks[i];
      task->stop();
      task->halt();
       //delete task;
    }
    tasks.clear();
  }
};

class MainBLEServer: public Task {
   BLEServer *pServer;

public:
  
  void run(void *data) {

    BLEDevice::init("M5 BlueTooth Keyboard");
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new ServerCallbacks());

    /*
     * https://www.bluetooth.com/specifications/gatt/viewer
     */   
    hid = new BLEHIDDevice(pServer);

    input = hid->inputReport(1); // <-- input REPORTID from report map
    output = hid->outputReport(1); // <-- output REPORTID from report map

    output->setCallbacks(new OutputCallbacks());
    std::string name = "_d";
    hid->manufacturer()->setValue(name);
    hid->pnp(0x02, 0xe502, 0xa111, 0x0210);
    hid->hidInfo(0x00,0x01);
    hid->reportMap((uint8_t*)reportMap, sizeof(reportMap));
    hid->setBatteryLevel(0x01);


    hid->startServices();

    /*
     * Its good to setup advertising by providing appearance and advertised service. This will let clients find our device by type
     */
    BLEAdvertising *pAdvertising = pServer->getAdvertising();
    pAdvertising->setAppearance(HID_KEYBOARD);
    pAdvertising->addServiceUUID(hid->hidService()->getUUID());
    pAdvertising->start();

    BLESecurity *pSecurity = new BLESecurity();
    pSecurity->setAuthenticationMode(ESP_LE_AUTH_BOND);//a

    Serial.println("server advertising");
    displayStatus("A");
    Serial.println(hid->hidService()->getUUID().toString().c_str());

    delay(1000000);
  }
};

MainBLEServer* pMainBleServer = NULL;

void StartBLEServer(void)
{
  pMainBleServer = new MainBLEServer();
  pMainBleServer->setStackSize(20000);
  pMainBleServer->start();
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
  if(isConnected && M5.BtnA.wasPressed()) {
    InputTask *task = new InputTask("Hello From M5Stack!\n");
    task->start();
    tasks.push_back(task);
  }

  if(isConnected && M5.BtnB.wasPressed()) {
    KEYMAP payload[1];
    payload[0] = {0x4c, KEY_CTRL | KEY_ALT}; // CTRL+ALT+DEL !!!!
    InputTask *task = new InputTask(payload, 1);
    task->start();
  }
  
  if (M5.BtnC.wasPressed()) {
    Serial.println("btn c");
    BLE2902* desc;
     desc = (BLE2902*) input->getDescriptorByUUID(BLEUUID((uint16_t)0x2902));
     desc->setNotifications(true);
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
