/*
    2019/1/22 by cp
*/


#ifndef SDCARD_H
#define SDCARD_H

#include "SDCard.h"
#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include <string>

namespace SDCard{
    bool isMounted=false;
    
    void mount();

    void ls(const char *path);
    bool mkdir(const char *path);
    bool rmdir(const char *path);
    std::string read(const char *path);
    bool write(const char *path,const char *message);
    bool append(const char *path,const char *message);
    bool mv(const char *from,const char *to);
    bool rm(const char *path);

    void displaySDStatus();
}

#endif