/*
    2019/1/21 by cp
*/
#include <M5Stack.h>
#include "BLEHIDKeyboard.h"

bool isConnected = false;

BLEHIDDevice *hid;
BLECharacteristic *input;
BLECharacteristic *output;
BLEServer *bleserver;
BLESecurity *security;
ServerCallbacks *servercallback;
OutputCallbacks *outputcallback;


void displayBLEServerStatus()
{
  if(isConnected){
    M5.Lcd.fillCircle(10,230,3,GREEN);
  }else{
    M5.Lcd.fillCircle(10,230,3,RED);
  }
}

void BLEServerTask::run(void *data) {

    BLEDevice::init("M5 BlueTooth Keyboard");
    bleserver = BLEDevice::createServer();
    servercallback=new ServerCallbacks();
    bleserver->setCallbacks(servercallback);

    /*
     * https://www.bluetooth.com/specifications/gatt/viewer
     */   
    hid = new BLEHIDDevice(bleserver);

    input = hid->inputReport(1); // <-- input REPORTID from report map
    output = hid->outputReport(1); // <-- output REPORTID from report map

    outputcallback=new OutputCallbacks();
    output->setCallbacks(outputcallback);
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

    security = new BLESecurity();
    security->setAuthenticationMode(ESP_LE_AUTH_BOND);//a

    Serial.println("server advertising");
    displayBLEServerStatus();
    Serial.println(hid->hidService()->getUUID().toString().c_str());

    // delay(1000000);
}



void ServerCallbacks::onConnect(BLEServer* bleserver){
     BLE2902* desc;
     desc = (BLE2902*) input->getDescriptorByUUID(BLEUUID((uint16_t)0x2902));
     desc->setNotifications(true);
     isConnected = true;
     displayBLEServerStatus();
  }

void ServerCallbacks::onDisconnect(BLEServer* bleserver){
    isConnected = false;
    displayBLEServerStatus();
}

/*
 * This callback is connect with output report. In keyboard output report report special keys changes, like CAPSLOCK, NUMLOCK
 * We can add digital pins with LED to show status
 * bit 1 - NUM LOCK
 * bit 2 - CAPS LOCK
 * bit 3 - SCROLL LOCK
 */
void OutputCallbacks::onWrite(BLECharacteristic* me){
    const char* value = me->getValue().c_str();
    M5.Lcd.print(value);
}

void simulateKey(KEYMAP map){
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
        simulateKey(map);
        vTaskDelay(100/portTICK_PERIOD_MS);
    }
}

BLEServerTask* bleservertask = NULL;

void StartBLEServer()
{
    if(isConnected){
        isConnected=false;
        displayBLEServerStatus();
        delete hid;
        delete input;
        delete output;
        delete bleserver;
        delete security;
        delete servercallback;
        delete outputcallback;
        delete bleservertask;
    }else{
        bleservertask = new BLEServerTask();
        bleservertask->setStackSize(20000);
        bleservertask->start();
    }
}

unsigned char modifierkeys=0;

void inputKeyValue(int key_val){
    if(isConnected){
        //clear mult keys
        if(key_val==176){//alt space 
            modifierkeys=0;
            return;
        }
        //push mult keys
        if(key_val>=144&&key_val<=153){//alt q ~ alt p
            modifierkeys |= m5keymap[key_val].modifier;
            return;
        }

        KEYMAP usagekey=m5keymap[(int)key_val];
        usagekey.modifier |= modifierkeys;
        modifierkeys=0;

        simulateKey(usagekey);
    }
}
