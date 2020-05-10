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
          40            120                 320x240

*/
#include <M5Stack.h>
#include "Display.h"

const String README[11]={
  "CTRL SHIFT ALT CMD L R CTRL SHIFT ALT CMD",
  " #    1    2    3    (    )    _    -    +    @  ",
  "~ q  ^ w  & e  ` r  < t  > y  { u   } i  [ o  ] p",
  "--------------------------------------------------",
  "F1   F2   F3   F4   F5  F6  F7   F8  F9",
  "                      PS   SL  CL   ^   INS  DEL",
  "|*a  =4s  \\5d  %6f  / g  : h  ; j   ' k  \"  l   BS",
  "--------------------------------------------------",
  "      F10 F11 F12                       time",
  "      TAB HOM END  PU   PD  <    v    > ",
  "ALT  7 z  8 x  9 c  ? v  ! b  , n  . m    $   LF"
  };

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

void Display::result(const char* result){
    M5.Lcd.fillRect(220,220,100,20,0x293573);
    M5.Lcd.setCursor(225,222);
    M5.Lcd.print(result);
}

void Display::info(const char* info){
    M5.Lcd.fillRect(120,220,220,20,0x293573);
    M5.Lcd.setCursor(125,222);
    M5.Lcd.print(info);
}

void Display::readme(){
    M5.Lcd.setCursor(5,10);
    M5.Lcd.setTextColor(0x664411);
    M5.Lcd.print(README[0].c_str());
    M5.Lcd.setCursor(5,30);
    M5.Lcd.setTextColor(0x000055);
    M5.Lcd.print(README[1].c_str());
    M5.Lcd.setCursor(5,50);
    M5.Lcd.setTextColor(0x554444);
    M5.Lcd.print(README[2].c_str());
    M5.Lcd.setCursor(5,65);
    M5.Lcd.setTextColor(0x221122);
    M5.Lcd.print(README[3].c_str());
    M5.Lcd.setCursor(5,80);
    M5.Lcd.setTextColor(0x664411);
    M5.Lcd.print(README[4].c_str());
    M5.Lcd.setCursor(5,100);
    M5.Lcd.setTextColor(0x8b8900);
    M5.Lcd.print(README[5].c_str());
    M5.Lcd.setCursor(5,120);
    M5.Lcd.setTextColor(0x554444);
    M5.Lcd.print(README[6].c_str());
    M5.Lcd.setCursor(5,135);
    M5.Lcd.setTextColor(0x221122);
    M5.Lcd.print(README[7].c_str());
    M5.Lcd.setCursor(5,150);
    M5.Lcd.setTextColor(0x664411);
    M5.Lcd.print(README[8].c_str());
    M5.Lcd.setCursor(5,170);
    M5.Lcd.setTextColor(0x8b8900);
    M5.Lcd.print(README[9].c_str());
    M5.Lcd.setCursor(5,190);
    M5.Lcd.setTextColor(0x554444);
    M5.Lcd.print(README[10].c_str());
    M5.Lcd.setTextColor(WHITE);
}

int cursor_x=5;
int cursor_y=2;

void Display::clear(){
    M5.Lcd.fillRect(0,0,320,220,BLACK);
    Display::readme();
    cursor_y=2;
    cursor_x=5;
    M5.Lcd.setCursor(cursor_x,cursor_y);
}

void Display::print(char c){
    int i=(int)c;
    if(i==8){//backspace
        M5.Lcd.fillRect(cursor_x,cursor_y,10,18,BLACK);
        cursor_x-=10;
        return;
    }
    if(i==13){//enter
        cursor_y+=18;
        cursor_x=5;
        return;
    }
    if(cursor_x>=300){
        cursor_x=5;
        cursor_y+=18;
    }else{
      cursor_x+=10;
    }
    if(cursor_y>=216){
        cursor_y=2;
        M5.Lcd.fillRect(0,0,320,220,BLACK);
    }
    M5.Lcd.setCursor(cursor_x,cursor_y);
    M5.Lcd.print(c);  
}
