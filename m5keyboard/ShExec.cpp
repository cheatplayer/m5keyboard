/*
    2019/1/31 by cp
    */

#include "ShExec.h"
#include "BLEHIDKeyboard.h"
#include "Display.h"

bool isrecord_press=false;
std::string record_press="";
std::string Sh::stringify(char key_val){
    int i=(int)key_val;
    if(isrecord_press&&i==176){
        record_press="";
        isrecord_press=false;
        return "";
    }
    if(isrecord_press&&i<144&&i>153){
        isrecord_press=false;
        std::string result=record_press+std::string(1,key_val)+"\r";
        record_press="";
        return result;
    }

    if(i==144&&!isrecord_press){
        isrecord_press=true;
        record_press+="\r>>> LCTRL ";
        return "";
    }else if(i==144&&isrecord_press){
        record_press+="LCTRL ";
        return "";
    }
    if(i==145&&!isrecord_press){
        isrecord_press=true;
        record_press+="\r>>> LSHIFT ";
        return "";
    }else if(i==145&&isrecord_press){
        record_press+="LSHIFT ";
        return "";
    }
    if(i==146&&!isrecord_press){
        isrecord_press=true;
        record_press+="\r>>> LALT ";
        return "";
    }else if(i==146&&isrecord_press){
        record_press+="LALT ";
        return "";
    }
    if(i==147&&!isrecord_press){
        isrecord_press=true;
        record_press+="\r>>> LMETA ";
        return "";
    }else if(i==147&&isrecord_press){
        record_press+="LMETA ";
        return "";
    }
    if(i==150&&!isrecord_press){
        isrecord_press=true;
        record_press+="\r>>> RCTRL ";
        return "";
    }else if(i==150&&isrecord_press){
        record_press+="RCTRL ";
        return "";
    }
    if(i==151&&!isrecord_press){
        isrecord_press=true;
        record_press+="\r>>> RSHIFT ";
        return "";
    }else if(i==151&&isrecord_press){
        record_press+="RSHIFT ";
        return "";
    }
    if(i==152&&!isrecord_press){
        isrecord_press=true;
        record_press+="\r>>> RALT ";
        return "";
    }else if(i==152&&isrecord_press){
        record_press+="RALT ";
        return "";
    }
    if(i==153&&!isrecord_press){
        isrecord_press=true;
        record_press+="\r>>> RMETA ";
        return "";
    }else if(i==153&&isrecord_press){
        record_press+="RMETA ";
        return "";
    }

    if(i==127){//del
        return "\r::: DEL\r";
    }

    // if(i>=154&&i<=161){//f1-f8
    //     std::string num=std::to_string(i-153);
    //     return "\r~~~ "+num+"\r";
    // }
    // if(i>=164&&i<=167){//f9-f12
    //     std::string num=std::to_string(i-155);
    //     return "\r~~~ "+num+"\r";
    // }

    if(i==163){
        return "\n\r";
    }
    if(i==172){//delay
        return "\r$$$ 1000\r";
    }

    if(i==175){
        return "\r::: ESC\r";
    }
    if(i==182){
        return "\r::: CAPS\r";
    }
    if(i==183){
        return "\r::: UP\r";
    }
    if(i==191){
        return "\r::: LEFT\r";
    }
    if(i==192){
        return "\r::: DOWN\r";
    }
    if(i==193){
        return "\r::: RIGHT\r";
    }

    //todo test tab /b /n

    return std::string(1,key_val);
}

std::vector<std::string> Sh::split(std::string str,char sep){
    std::vector<std::string> result;
    std::string::size_type pos1, pos2;
    pos2 = str.find(sep);
    pos1 = 0;
    while (std::string::npos != pos2)
    {
            result.push_back(str.substr(pos1, pos2 - pos1));

            pos1 = pos2 + 1;
            pos2 = str.find(sep, pos1);
    }
    result.push_back(str.substr(pos1));
    return result;
}

Exec::Exec(std::string text){
    setSplit(text);
}

void Exec::setSplit(std::string text){
    lines=Sh::split(text,'\r');
    length=lines.size();
}

extern void simulateKey(KEYMAP map);
extern bool isConnected;

void Exec::execBLEString(const char *text){
    if(isConnected){
        const char *pointer=text;
        while(*pointer){
            KEYMAP map=keymap[(uint8_t)*pointer];
            simulateKey(map);
            vTaskDelay(100/portTICK_PERIOD_MS);
            pointer++;
        }
    }

}

void Exec::execLine(std::string line){
    // Serial.println(line.c_str());
//    if(line.substring(0,3)==">>>"){
//
//    }else if(line.substring(0,3)=="$$$"){
//        vTaskDelay(line.substring(4).toInt())
//    }else if(line.substring(0,3)=="~~~"){
//
//    }else if(line.substring(0,3)==":::"){
//
//    }else{
        execBLEString(line.c_str());
//    }
}

void Exec::run(void*){
    int i=0;
    while(i<length){
        execLine(lines[i]);
        i++;
    }
    Display::result("hack ok");
}
