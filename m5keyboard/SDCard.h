/*
    2019/1/22 by cp
*/


#ifndef SDCARD_H
#define SDCARD_H

namespace SDCard{
    bool isMounted=false;
    
    void mount();
    void displaySDStatus();
}

#endif