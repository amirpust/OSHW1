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
    ~SmallShell(){
        close(stdIn);
        close(stdOut);
        close(stdErr);
    };
    void executeCommand(const char* cmd_line);
    Command* createCommand(string& original,string& cmdLine,string* splitCmd, int size);
    void setName(const string& newName);
    const string &getName() const;
    const string &getPreviousDir() const;
    void cd(const string& str);
    pid_t getMyPid() const;

private:
    SmallShell();
    void cmdDecryptor(const string& original,string* cmd,string* splitCmd,int* size, bool* bg);
    int parseCommand(const string& cmd, string* splitCmd);
    bool isBackground(const string& cmd);
    void removeBackgroundSign(string& cmd);
    redirectionType identifyRedirection(string* splitCmd, int size, string* path);
    void prepareIO(redirectionType type, string path);
    void cleanUpIO(pid_t pipePid);
    int checkPipe(string split[], int size, pid_t* enterPid){
        PRINT_START;

        for(int i = 0; i < size; i++){
            if(split[i]  == "|"){
                splitPipe(pipeRegular, enterPid);
                return i;

            }else if(split[i] == "|&"){
                splitPipe(pipeStderr, enterPid);
                return i;
            }
        }
        PRINT_END;
        return -1;
    }
    void splitPipe(pipeType type, pid_t* enterPid){
        PRINT_START;
        fileDescriptor pipeFD[2];
        if (pipe(pipeFD) != 0)
            exit(-1); //TODO: handle properly

        (*enterPid) = fork();

        if ((*enterPid) == 0){                               //Enter (write to pipe)
            if(type == pipeRegular){
                close(1);
            }else{
                close(2);
            }
            dup(pipeFD[1]);



        }else{                                              //Exit (read from pipe)
            close(0);
            dup(pipeFD[0]);
        }
        PRINT_END;
    }
};


#endif //OSHW1_SMALLSHELL_H
