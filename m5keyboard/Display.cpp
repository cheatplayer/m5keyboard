/*
    2019/1/23 by cp
 5x2 10x18 31x12
+-------------------------------------------------+
|                                          320x220|
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
| * *     menu name     menu info                 |
+-------------------------------------------------+
          40            100                 320x240

*/
#include <M5Stack.h>
#include "Display.h"

void Display::init(){
    M5.Lcd.setTextFont(2);
    M5.Lcd.setTextColor(WHITE);
    // M5.lcd.setBrightness(5);
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.fillRect(0,220,320,20,0x293573);
}

void Display::menu(const char* menuname){
    M5.Lcd.fillRect(40,220,200,20,0x293573);
    M5.Lcd.setCursor(45,222);
    M5.Lcd.print(menuname);
}

void Display::info(const char* info){
    M5.Lcd.fillRect(90,200,140,20,0x293573);
    M5.Lcd.setCursor(95,222);
    M5.Lcd.print(info);
}

int cursor_x=5;
int cursor_y=2;

void Display::clear(){
    M5.Lcd.fillRect(0,0,320,220,BLACK);
    cursor_y=2;
    cursor_x=5;
    M5.Lcd.setCursor(cursor_x,cursor_y);
}

void Display::print(char c){
    int i=(int)c;
    if(i==8){//backspace
        M5.Lcd.fillRect(cursor_x,cursor_y,10,18,BLACK);
        return;
    }
    if(i==13){//enter
        cursor_y+=18;
        cursor_x=5;
        return;
    }
    if(cursor_x>=310){
        cursor_x=5;
        cursor_y+=18;
    }
    if(cursor_y>=216){
        cursor_y=2;
        M5.Lcd.fillRect(0,0,320,220,BLACK);
    }
    M5.Lcd.setCursor(cursor_x,cursor_y);
    M5.Lcd.print(c);  
}
