/*
    2019/1/31 by cp
    */



#ifndef SHEXEC_H
#define SHEXEC_H

#include "Task.h"
#include <vector>

namespace Sh{
    String stringify(char key_val);
    std::vector<String> split(String str,char sep);
    int parse(String keyname);
}

class Exec:public Task
{
public:
    std::vector<String> lines;
    int length=0;
    bool isloop=false;
    Exec(String text);
    Exec();
    void setSplit(String text);
    void run(void*);
    void execLine(String line);
    void execBLEString(const char *text); 
};

#endif