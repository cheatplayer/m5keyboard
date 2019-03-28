/*
    2019/1/21 by cp
*/
#include <M5Stack.h>
#include "BLEHIDKeyboard.h"
#include "Display.h"

bool isConnected = false;
bool isUseMouse=true;

BLEHIDDevice *hid;
BLECharacteristic *input;
BLECharacteristic* inputMouse;
BLECharacteristic *output;
BLEServer *bleserver;
// BLESecurity *security;
ServerCallbacks *servercallback;
OutputCallbacks *outputcallback;
BLEAdvertising *advertising;


void displayBLEServerStatus()
{
  if(isConnected){
    M5.Lcd.fillCircle(10,230,3,BLUE);
  }else{
    M5.Lcd.fillCircle(10,230,3,RED);
  }
}

void ServerCallbacks::onConnect(BLEServer* bleserver){
     BLE2902* desc;
     desc = (BLE2902*) input->getDescriptorByUUID(BLEUUID((uint16_t)0x2902));
     desc->setNotifications(true);
     isConnected = true;
     displayBLEServerStatus();
}

void ServerCallbacks::onDisconnect(BLEServer* bleserver){
    BLE2902* desc;
    desc = (BLE2902*) input->getDescriptorByUUID(BLEUUID((uint16_t)0x2902));
    desc->setNotifications(false);
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
    Serial.print("output callback");
}

void simulateKey(KEYMAP map){
    if(isConnected){
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

}

void StartBLEServer()
{

    BLEDevice::init("M5 BlueTooth HID");
    bleserver = BLEDevice::createServer();
    servercallback=new ServerCallbacks();
    bleserver->setCallbacks(servercallback);

    /*
     * https://www.bluetooth.com/specifications/gatt/viewer
     */   
    hid = new BLEHIDDevice(bleserver);
    std::string name = "cheatplayer";
    hid->manufacturer()->setValue(name);
    hid->pnp(0x02, 0xe502, 0xa111, 0x0210);
    hid->hidInfo(0x00,0x01);

    // hid->reportMap((uint8_t*)reportMapOld, sizeof(reportMapOld));//old
    // hid->setBatteryLevel(0x01);

    size_t reportMapSize = sizeof(reportMapKeyboard);
    if(isUseMouse)reportMapSize+=sizeof(reportMapMouse);
    uint8_t *reportMap = (uint8_t *)malloc(reportMapSize);
    uint8_t *reportMapCurrent = reportMap;
    uint8_t reportID = 1;
    //keyboard
    memcpy(reportMapCurrent,reportMapKeyboard,sizeof(reportMapKeyboard));
    reportMap[7] = reportID;
    reportMapCurrent += sizeof(reportMapKeyboard);
    input = hid->inputReport(reportID); // <-- input REPORTID from report map
    output = hid->outputReport(reportID); // <-- output REPORTID from report map
    outputcallback=new OutputCallbacks();
    output->setCallbacks(outputcallback);
    //mouse
    if(isUseMouse){
        reportID++;
        memcpy(reportMapCurrent,reportMapMouse,sizeof(reportMapMouse));
        reportMapCurrent[7] = reportID;
        reportMapCurrent += sizeof(reportMapMouse);
        
        //create in characteristics/reports for mouse
        inputMouse = hid->inputReport(reportID);
    }

    hid->reportMap((uint8_t*)reportMap, reportMapSize);
    hid->startServices();

    /*
     * Its good to setup advertising by providing appearance and advertised service. This will let clients find our device by type
     */
    advertising = bleserver->getAdvertising();
    // advertising->setAppearance(HID_KEYBOARD);//old
    advertising->setAppearance(GENERIC_HID);
    advertising->addServiceUUID(hid->hidService()->getUUID());
    advertising->start();

    // security = new BLESecurity();
    // security->setAuthenticationMode(ESP_LE_AUTH_BOND);//a

    Serial.println("server advertising");
    displayBLEServerStatus();
    Serial.println(hid->hidService()->getUUID().toString().c_str());
}

void StopBLEServer(){
    if(isConnected){
        advertising->stop();
        hid->deviceInfo()->stop();
        hid->hidService()->stop();
        hid->batteryService()->stop();
        isConnected=false;
        displayBLEServerStatus();
        delete hid;
        delete input;
        delete output;
        delete bleserver;
        // delete security;
        delete servercallback;
        delete outputcallback;
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

