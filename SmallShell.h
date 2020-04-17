#ifndef OSHW1_SMALLSHELL_H
#define OSHW1_SMALLSHELL_H

#include <sstream>
#include <sys/stat.h>
#include "Libs.h"
#include "JobsList.h"

const std::string WHITESPACE = " \n\r\t\f\v";
typedef enum{
    enterPipe , exitPipe
} pipeSide;
typedef int fileDescriptor;

class SmallShell {
    JobsList& jobs;
    const string defaultName;
    std::string name;
    string currentDir;
    string previousDir;
    pid_t myPid;
    fileDescriptor stdIn;
    fileDescriptor stdOut;
    fileDescriptor stdErr;

public:

    SmallShell(SmallShell const&)      = delete; // disable copy ctor
    void operator=(SmallShell const&)  = delete; // disable = operator
    static SmallShell& getInstance();
    ~SmallShell();;
    void executeCommand(const char* cmd_line);
    Command* createCommand(string& original,string& cmdLine,string* splitCmd, int size);
    void setName(const string& newName);
    const string &getName() const;
    const string &getPreviousDir() const;
    void cd(const string& str);
    pid_t getMyPid() const;
    void cleanUpIO(pid_t pipePid);

private:

    SmallShell();
    void cmdDecryptor(const string& original,string* cmd,string* splitCmd,int* size, bool* bg);
    int parseCommand(const string& cmd, string* splitCmd);
    bool isBackground(const string& cmd);
    void removeBackgroundSign(string& cmd);
    redirectionType identifyRedirection(string* splitCmd, int size, string* path);
    void prepareIO(redirectionType type, const string& path);
    int checkPipe(string split[], int size, pid_t* enterPid);
    void splitPipe(pipeType type, pid_t* enterPid);
    string cmdToString(const char* cmdLine){
        if (!cmdLine)
            return "";
        string newCmd = "";
        for ( int i = 0; cmdLine[i] != '\0'; i++){
            if(cmdLine[i] == '>'){
                if(cmdLine[i+1] == '>'){
                    newCmd.append(" >> ");
                    i++;
                }else{
                    newCmd.append(" > ");
                }
            }else if(cmdLine[i] == '|'){
                if(cmdLine[i+1] == '&'){
                    newCmd += " |& ";
                    i++;
                }else{
                    newCmd += " | ";
                }
            }else{
                newCmd += cmdLine[i];
            }
        }
        return newCmd;
    }
};


#endif //OSHW1_SMALLSHELL_H
