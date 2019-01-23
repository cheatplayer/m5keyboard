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

void Display::init(){
    M5.Lcd.setTextFont(2);
    M5.Lcd.setTextColor(WHITE);
    M5.lcd.setBrightness(100);
    M5.Lcd.fillScreen(0x222222);
    M5.Lcd.fillRect(0,220,320,20,0x293573)
}

void Display::menu(const char* menuname){
    M5.Lcd.fillRect(50,220,170,20,0x293573);
    M5.Lcd.setCursor(55,220);
    M5.Lcd.print(menuname);
    M5.Lcd.setCursor(0,0);
}

void Display::clear(){
    M5.Lcd.fillRect(0,0,320,220,0x222222);
}