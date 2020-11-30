/*
    2019/2/27 by cp
*/

#include "Arduino.h"

#ifndef SDCARD_H
#define SDCARD_H

namespace SDCard{
    bool write(const char *path,const char *text);
    bool append(const char *path,const char *text);
    bool rm(const char *path);
    String ls(const char *path);
    String read(const char *path);
}

#endif