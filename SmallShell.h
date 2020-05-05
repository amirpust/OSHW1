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
    bool running;

public:

    SmallShell(SmallShell const&)      = delete; // disable copy ctor
    void operator=(SmallShell const&)  = delete; // disable = operator
    static SmallShell& getInstance();
    ~SmallShell();
    void executeCommand(const char* cmd_line);
    Command* createCommand(string* splitCmd, int size);
    void setName(const string& newName);
    const string &getName() const;
    const string &getPreviousDir() const;
    void cd(const string& str);
    pid_t getMyPid() const;
    void cleanUpIO();

    bool isRunning() const;

    void setRunning(bool running);

private:

    SmallShell();
    void cmdDecryptor(const string& original,string* cmd,string* splitCmd,int* size, bool* bg);
    int parseCommand(const string& cmd, string* splitCmd);
    bool isBackground(const string& cmd);
    bool isTimeout(string* splitCmd);
    void removeBackgroundSign(string& cmd);
    void removeRedirection(string& cmd);
    redirectionType identifyRedirection(string* splitCmd, int size, string* path);
    void prepareIO(redirectionType type, const string& path, int* size);
    int checkPipe(string split[], int size, pid_t *leftPid, pid_t *rightPid);
    void splitPipe(pipeType type, pid_t *leftPid, pid_t *rightPid);
    string cmdToString(const char* cmdLine);
};


#endif //OSHW1_SMALLSHELL_H
