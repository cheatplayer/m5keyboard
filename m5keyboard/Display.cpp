/*
    2019/1/23 by cp
+-------------------------------------------------+
|                                          320*220|
|                                                 |
|                                                 |
|                                                 |
|                                                 |
|                                                 |
|                                                 |
|                                                 |
|                                                 |
|                                                 |
|                                                 |
|                                                 |
|                                                 |
|                                                 |
+-------------------------------------------------+220
| * *       menu name     menu info               |
+-------------------------------------------------+
            50                              320x240

*/
#include <M5Stack.h>
#include "Display.h"

void Display::init(){
    M5.Lcd.setTextFont(2);
    M5.Lcd.setTextColor(WHITE);
    M5.lcd.setBrightness(30);
    M5.Lcd.fillScreen(0x050505);
    M5.Lcd.fillRect(0,220,320,20,0x293573);
}

void Display::menu(const char* menuname){
    M5.Lcd.fillRect(50,220,170,20,0x293573);
    M5.Lcd.setCursor(55,222);
    M5.Lcd.print(menuname);
    M5.Lcd.setCursor(0,0);
}

void Display::clear(){
    M5.Lcd.fillRect(0,0,320,220,0x050505);
}
