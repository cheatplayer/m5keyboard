// 2019/3/28 by cp

#include "BLEHIDKeyboard.h"
#include "BLEHIDMouse.h"

bool isMouseOn=false;
extern bool isConnected;
extern BLECharacteristic* inputMouse;


void doMouse(uint8_t button,uint8_t x,uint8_t y,uint8_t wheel){
    if(isConnected && isMouseOn){
        uint8_t a[]={button,x,y,wheel};
        inputMouse->setValue(a,sizeof(a));
        inputMouse->notify();
    }

}

void Mouse::clickLeft(){
    doMouse(0x01,0,0,0);
}

void Mouse::clickRight(){
    doMouse(0x02,0,0,0);
}

void Mouse::wheel(uint8_t wheel){
    doMouse(0,0,0,wheel);
}

void Mouse::move(uint8_t x,uint8_t y){
    doMouse(0,x,y,0);
}

void Mouse::test(){
    doMouse(0,20,20,0);
}