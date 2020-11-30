// 2019/2/27 by cp
#include "SDCard.h"
#include <SD.h>

String SDCard::read(const char *path){
    String result="";
    File f=SD.open(path);
    if(f){
        while(f.available()){
            result+=(char)f.read();
        }
        f.close();
    }
    return result;
}

bool SDCard::write(const char *path,const char *text){
    SDCard::rm(path);
    File f=SD.open(path,FILE_WRITE);
    if(f){
        f.println(text);
        f.close();
        return true;
    }else{
        return false;
    }
}

bool SDCard::append(const char *path,const char *text){
    File f=SD.open(path,FILE_WRITE);
    if(f){
        f.println(text);
        f.close();
        return true;
    }else{
        return false;
    }
}

bool SDCard::rm(const char *path){
    if(SD.exists(path)){
        SD.remove(path);
        return true;
    }else{
        return false;
    }
}

String SDCard::ls(const char *path){
    String r="";
    File root = SD.open(path);
    if(!root||!root.isDirectory()){
        return r;
    }
    File file=root.openNextFile();
    while(file){
        String filename=file.name();
        if(file.isDirectory()){
        }else{
            r+=filename+"\r";
        }
        file=root.openNextFile();
    }
    root.close();
    return r;
}