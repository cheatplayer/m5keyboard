/*
    2019/1/31 by cp
    */

#include "ShExec.h"
#include "BLEHIDKeyboard.h"
#include "Display.h"

const std::string Fmap[12]={
    "F1",
    "F2",
    "F3",
    "F4",
    "F5",
    "F6",
    "F7",
    "F8",
    "F9",
    "F10",
    "F11",
    "F12"
};

bool isrecord_press=false;
std::string record_press="";
std::string Sh::stringify(char key_val){
    int i=(int)key_val;
    if(isrecord_press&&i==176){
        record_press="";
        isrecord_press=false;
        return "";
    }
    if(isrecord_press&&i==32){
        isrecord_press=false;
        std::string result=record_press+"SPACE\r";
        record_press="";
        return result;
    }
    if(isrecord_press&&(i<144||i>153)){
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

    if(i==13)return "\r::: ENTER\r";
    if(i==127)return "\r::: DEL\r";
    if(i>=154&&i<=161)return "\r::: "+Fmap[i-156]+"\r";
    if(i>=164&&i<=167)return "\r::: "+Fmap[i-156]+"\r";
    if(i==163)return "\n\r";   
    if(i==172)return "\r$$$ 1000\r";
    if(i==175)return "\r::: ESC\r"; 
    if(i==182)return "\r::: CAPS\r";
    if(i==183)return "\r::: UP\r";
    if(i==191)return "\r::: LEFT\r";
    if(i==192)return "\r::: DOWN\r";
    if(i==193)return "\r::: RIGHT\r";

    return std::string(1,key_val);
}

int Sh::parse(std::string n){
    if(n=="ENTER")return 13;
    if(n=="LCTRL"||n=="CTRL")return 144;
    if(n=="LSHIFT"||n=="SHIFT")return 145;
    if(n=="LALT"||n=="ALT")return 146;
    if(n=="LMETA"||n=="META"||n=="GUI"||n=="CMD")return 147;
    if(n=="RCTRL")return 150;
    if(n=="RSHIFT")return 151;
    if(n=="RALT") return 152;
    if(n=="RMETA")return 153;
    if(n=="DEL")return 127;
    if(n=="F1")return 154;
    if(n=="F2")return 155;
    if(n=="F3")return 156;
    if(n=="F4")return 157;
    if(n=="F5")return 158;
    if(n=="F6")return 159;
    if(n=="F7")return 160;
    if(n=="F8")return 161;
    if(n=="F9")return 164;
    if(n=="F10")return 165;
    if(n=="F11")return 166;
    if(n=="F12")return 167;
    if(n=="ESC")return 175;
    if(n=="CAPS")return 182;
    if(n=="UP")return 183;
    if(n=="LEFT")return 191;
    if(n=="DOWN")return 192;
    if(n=="RIGHT")return 193;
    if(n=="SPACE")return 32;
    return (int)n[0];
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
extern void inputKeyValue(int key_val);

void Exec::execBLEString(const char *text){
    if(isConnected){
        const char *pointer=text;
        while(*pointer){
            KEYMAP map=m5keymap[(uint8_t)*pointer];
            simulateKey(map);
            vTaskDelay(100/portTICK_PERIOD_MS);
            pointer++;
        }
    }
}

extern unsigned char modifierkeys;

void Exec::execLine(std::string line){
    // Serial.println(line.c_str());
   if(line.substr(0,3)==">>>"){
        std::vector<std::string> sp=Sh::split(line.substr(4),' ');
        int i=0;
        modifierkeys=0;
        while(i<sp.size()){
                int key=Sh::parse(sp[i]);
                inputKeyValue(key);
                i++;
            }
   }else if(line.substr(0,3)=="$$$"){
        int n=atoi(line.substr(4).c_str());
        vTaskDelay(n);
   }else if(line.substr(0,3)==":::"){
        int key=Sh::parse(line.substr(4));
        inputKeyValue(key);
   }else{
        execBLEString(line.c_str());
   }
}

void Exec::run(void*){
    int i=0;
    while(i<length){
        execLine(lines[i]);
        i++;
    }
    Display::result("hack ok");
}
