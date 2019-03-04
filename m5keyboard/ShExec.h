/*
    2019/1/31 by cp
    */



#ifndef SHEXEC_H
#define SHEXEC_H

#include <string>
#include "Task.h"
#include <vector>

namespace Sh{
    std::string stringify(char key_val);
    std::vector<std::string> split(std::string str,char sep);
    int parse(std::string keyname);
}

class Exec:public Task
{
public:
    std::vector<std::string> lines;
    int length=0;
    bool isloop=false;
    Exec(std::string text);
    Exec();
    void setSplit(std::string text);
    void run(void*);
    void execLine(std::string line);
    void execBLEString(const char *text); 
};

#endif