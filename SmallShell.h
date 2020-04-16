#ifndef OSHW1_SMALLSHELL_H
#define OSHW1_SMALLSHELL_H

#include <sstream>
#include "Libs.h"
#include "JobsList.h"

const std::string WHITESPACE = " \n\r\t\f\v";

class SmallShell {
    JobsList& jobs;
    const string defaultName;
    std::string name;
    string currentDir;
    string previousDir;
    pid_t myPid;

public:

    SmallShell(SmallShell const&)      = delete; // disable copy ctor
    void operator=(SmallShell const&)  = delete; // disable = operator
    static SmallShell& getInstance();
    ~SmallShell() = default;
    void executeCommand(const char* cmd_line);
    Command* createCommand(string& original,string& cmdLine,string* splitCmd, int size);
    void setName(const string& newName);
    const string &getName() const;
    const string &getPreviousDir() const;
    void cd(const string& str){
        if(str == "-"){
            chdir(previousDir.c_str());
            string temp = previousDir;
            currentDir = previousDir;
            previousDir = temp;
        }else{
            previousDir = currentDir;
            chdir(str.c_str());
            char* temp = get_current_dir_name();
            currentDir = string(temp);
            free(temp);
        }
        //TODO : check if success
    }

private:
    SmallShell();
    void cmdDecryptor(const string& original,string* cmd,string* splitCmd,int* size, bool* bg);
    int parseCommand(const string& cmd,string* splitCmd);
    bool isBackground(const string& cmd);
    void removeBackgroundSign(string& cmd);

};


#endif //OSHW1_SMALLSHELL_H
