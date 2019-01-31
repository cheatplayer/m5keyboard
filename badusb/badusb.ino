/*
    2019/1/31 by cp
*/

#include "Keyboard.h"

void setup(){
    pinMode(LED_BUILTIN, OUTPUT);
    Keyboard.begin();
    delay(1000);
    delay(1000);
    Keyboard.press(KEY_RETURN);
    Keyboard.release(KEY_RETURN);
    Keyboard.press((uint8_t)'c');
    Keyboard.release((uint8_t)'c');
    Keyboard.write((uint8_t)'w');
    Keyboard.write((uint8_t)'w');
    Keyboard.write((uint8_t)'\b');
    Keyboard.write((uint8_t)'\n');
    Keyboard.write("hello",5);
    delay(1000);
    Keyboard.releaseAll();
    //Keyboard.end();
    digitalWrite(LED_BUILTIN, HIGH);
}

void loop(){

    delay(10000);

}
