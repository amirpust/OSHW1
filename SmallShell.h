#ifndef OSHW1_SMALLSHELL_H
#define OSHW1_SMALLSHELL_H

#include <sstream>
#include "Libs.h"
#include "JobsList.h"
#include "BuiltInCommand.h"


const std::string WHITESPACE = " \n\r\t\f\v";

class SmallShell {
    JobsList& jobs;
    const string defaultName;
    std::string name;
    string currentDir;
    string previousDir;
    pid_t myPid;
    //TODO : check if pre = null works
    SmallShell() : jobs(JobsList::getInstance()), defaultName("smash"), name(defaultName),previousDir(""){
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
        PRINT_START;
         //hope it will work
        bool bg;
        string original(cmd_line);
        string cmdStr;
        string splitCmd[COMMAND_MAX_ARGS];
        int size = 0;
        cmdDecryptor(original,&cmdStr,splitCmd,&size,&bg);

        Command* cmd = createCommand(original, cmdStr, splitCmd, size);

        if (cmd->getType() == builtIn){
            cmd->execute();
        }else{
            pid_t childPid = fork();
            if(childPid == 0){ //Child
                setpgrp();
                cmd->execute();
                exit(-1);
            }else{
                jobs.addJob((*(dynamic_cast<ExternalCommand*>(cmd))),childPid,"./",bg);
            }
        }
        PRINT_END;
    }

    Command* createCommand(string& original,string& cmdLine,string* splitCmd, int size){
        const std::string commands [] = {
                "chprompt", "showpid", "pwd", "cd", "jobs", "kill", "fg",
                "bg", "quit"
        };
     /*
        if (str.find(commands[0]) == 0)
            return new chpromptCommand(cmd_line,og_line);

        if (str.find(commands[1]) == 0)
            return new showpidCommand(cmd_line,og_line);

        if (str.find(commands[2]) == 0)
            return new pwdCommand(cmd_line,og_line);

        if (str.find(commands[3]) == 0)
            return new cdCommand(cmd_line,og_line);
        */
        if (original.find(commands[4]) == 0)
            return new JobsCommand(cmdLine,original,splitCmd,size);

 /*       if (str.find(commands[5]) == 0)
            return new killCommand(cmd_line,og_line);

        if (str.find(commands[6]) == 0)
            return new fgCommand(cmd_line,og_line);

        if (str.find(commands[7]) == 0)
            return new bgCommand(cmd_line,og_line);

        if (str.find(commands[8]) == 0)
            return new quitCommand(cmd_line,og_line);
*/
        return new ExternalCommand(cmdLine,original,splitCmd,size);
    }
    const string &getName() const {
        return name;
    }

};


#endif //OSHW1_SMALLSHELL_H
