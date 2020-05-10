/*
    2019/1/23 by cp
*/


#ifndef Menu_H
#define Menu_H



namespace Menu{
    void halt();
    void record(char key_val);
    void clear();
    void save();
    void load();
    void runScript();
    void runScriptStop();
    void runMenu();
    void loop();
    void loopStop();
    void loopMenu();
    void rm();
    void find();
    void rels();
    void ls();
    void startSTAMenu();
    void startAPMenu();
    void startClientMenu();
    void stopClientMenu();
    void stopAll();
    void clientCmd();
    void sendClientCmd();
}

#endif
