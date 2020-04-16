#ifndef OSHW1_SMALLSHELL_H
#define OSHW1_SMALLSHELL_H

#include <sstream>
#include "Libs.h"
#include "JobsList.h"
#include "BuiltInCommand.h"

const std::string WHITESPACE = " \n\r\t\f\v";

class SmallShell {
    JobsList jobs;
    const string defaultName;
    std::string name;
    string currentDir;
    string previousDir;
    pid_t myPid;
    //TODO : check if pre = null works
    SmallShell() : jobs(), defaultName("smash"), name(defaultName),previousDir(nullptr){
        currentDir = get_current_dir_name();
        myPid = getpid();
    };

    //maybe will return something else
    void cmdDecryptor(const string& original,string* cmd,string* splitCmd,int* size,
        bool* bg){
        *cmd = string(original);        // will copy the original cmd to make changes in it
        *bg = isBackground(original);   // checks if there is '&'
        removeBackgroundSign(*cmd);     // removes '&'
        *size = parseCommand(*cmd,splitCmd); // parses the cmd hopefully
    }

    int parseCommand(const string& cmd,string* splitCmd){
        int i = 0;
        istringstream iss(cmd);
        string str;
        while(getline(iss,str,' ')){
            splitCmd[++i] = string(str);
        }
        return i;
    }

    bool isBackground(const string& cmd){
        return cmd[cmd.find_last_not_of(WHITESPACE)] == '&';
    }

    void removeBackgroundSign(string& cmd){
        int index = cmd.find_last_not_of(WHITESPACE);
        if(cmd[index] != '&')
            return;

        cmd[index] = '\0';
    }

public:
    SmallShell(SmallShell const&)      = delete; // disable copy ctor
    void operator=(SmallShell const&)  = delete; // disable = operator
    static SmallShell& getInstance() // make SmallShell singleton
    {
        static SmallShell instance; // Guaranteed to be destroyed.
        // Instantiated on first use.
        return instance;
    }
    ~SmallShell() = default;

    void executeCommand(const char* cmd_line){
        bool bg;
        string original(cmd_line);
        string cmdStr;
        string splitCmd[COMMAND_MAX_ARGS];
        int size = 0;
        cmdDecryptor(original,&cmdStr,splitCmd,&size,&bg); //hope it will work

    }

};


#endif //OSHW1_SMALLSHELL_H
