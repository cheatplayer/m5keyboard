// 2019/2/27 by cp
#include "SDCard.h"
#include <SD.h>

std::string SDCard::read(const char *path){
    std::string result="";
    File f=SD.open(path);
    if(f){
        while(f.available()){
            result+=f.read();
        }
        f.close();
    }
    return result;
}

bool SDCard::write(const char *path,const char *text){
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

std::vector<std::string> SDCard::ls(const char *path){
    std::vector<std::string> v;
    File root = SD.open(path);
    if(!root||!root.isDirectory()){
        return v;
    }
    File file=root.openNextFile();
    while(file){
        std::string filename=file.name();
        if(file.isDirectory()){
            // v.push_back(filename+"/");
        }else{
            v.push_back(filename);
        }
        file=root.openNextFile();
    }
    root.close();
    return v;
}