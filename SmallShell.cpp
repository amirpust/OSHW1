#include "SmallShell.h"
#include "BuiltInCommand.h"


SmallShell &SmallShell::getInstance() {
    static SmallShell instance; // Guaranteed to be destroyed.
    // Instantiated on first use.
    return instance;
}

void SmallShell::executeCommand(const char *cmd_line) {
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
        ExternalCommand externalCommand(*dynamic_cast<ExternalCommand*>(cmd));
        delete cmd;
        if(childPid == 0){ //Child
            setpgrp();
            externalCommand.execute();
            exit(-1);
        }else {
            jobs.addJob((*(dynamic_cast<ExternalCommand *>(cmd))), childPid,
                        "./", bg);
        }
    }
    PRINT_END;
}

void SmallShell::setName(const string &newName) {
    if(newName.empty())
        name = defaultName;
    else
        name = newName;
}

Command *
SmallShell::createCommand(string &original, string &cmdLine, string *splitCmd,
                          int size) {
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

const string &SmallShell::getName() const {
    return name;
}

SmallShell::SmallShell() : jobs(JobsList::getInstance()), defaultName("smash"), name(defaultName),previousDir(""){
    char* temp = get_current_dir_name();
    currentDir = string(temp);
    free(temp);
    myPid = getpid();

}

void
SmallShell::cmdDecryptor(const string &original, string *cmd, string *splitCmd,
                         int *size, bool *bg) {
    *cmd = string(original);        // will copy the original cmd to make changes in it
    *bg = isBackground(original);   // checks if there is '&'
    removeBackgroundSign(*cmd);     // removes '&'
    *size = parseCommand(*cmd,splitCmd); // parses the cmd hopefully
}

int SmallShell::parseCommand(const string &cmd, string *splitCmd) {
    int i = 0;
    istringstream iss(cmd);
    string str;
    while(getline(iss,str,' ')){
        splitCmd[++i] = string(str);
    }
    return i;
}

bool SmallShell::isBackground(const string &cmd) {
    return cmd[cmd.find_last_not_of(WHITESPACE)] == '&';
}

void SmallShell::removeBackgroundSign(string &cmd) {
    int index = cmd.find_last_not_of(WHITESPACE);
    if(cmd[index] != '&')
        return;

    cmd[index] = '\0';
}

const string &SmallShell::getPreviousDir() const {
    return previousDir;
}
