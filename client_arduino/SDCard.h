/*
    2019/2/27 by cp
*/

#include <ArduinoSTL.h>

#ifndef SDCARD_H
#define SDCARD_H

namespace SDCard{
    bool write(const char *path,const char *text);
    bool rm(const char *path);
    std::vector<std::string> ls(const char *path);
    std::string read(const char *path);
}

#endif