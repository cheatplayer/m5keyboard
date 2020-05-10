/*
    2019/1/23 by cp
*/

#ifndef DISPLAY_H
#define DISPLAY_H

namespace Display{
    void init();
    void menu(const char* menuname);
    void clear();
    void info(const char* info);
    void result(const char* result);
    void print(char c);
    void readme();
}

#endif
