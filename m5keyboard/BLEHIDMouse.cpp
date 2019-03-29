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
    doMouse((uint8_t)1,(uint8_t)0,(uint8_t)0,(uint8_t)0);
}

void Mouse::clickRight(){
    doMouse((uint8_t)2,(uint8_t)0,(uint8_t)0,(uint8_t)0);
}

void Mouse::wheel(int wheel){
    doMouse((uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)wheel);
}

void Mouse::move(int x,int y){
    doMouse((uint8_t)0,(uint8_t)x,(uint8_t)y,(uint8_t)0);
}

void Mouse::test(){
    doMouse((uint8_t)0,(uint8_t)20,(uint8_t)20,(uint8_t)0);
}