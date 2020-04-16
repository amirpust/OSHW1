#include "SmallShell.h"
#include "BuiltInCommand.h"


SmallShell &SmallShell::getInstance() {
    static SmallShell instance; // Guaranteed to be destroyed.
    // Instantiated on first use.
    return instance;
}

void SmallShell::executeCommand(const char *cmd_line) {
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
            jobs.addJob(externalCommand, childPid, "./", bg);
        }
    }
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

    if (original.find(commands[0]) == 0)
        return new ChpromptCommand(cmdLine,original,splitCmd,size);

    if (original.find(commands[1]) == 0)
        return new ShowpidCommand(cmdLine,original,splitCmd,size);

    if (original.find(commands[2]) == 0)
        return new PwdCommand(cmdLine,original,splitCmd,size);

    if (original.find(commands[3]) == 0)
        return new CdCommand(cmdLine,original,splitCmd,size);

    if (original.find(commands[4]) == 0)
        return new JobsCommand(cmdLine,original,splitCmd,size);

    if (original.find(commands[5]) == 0)
        return new KillCommand(cmdLine,original,splitCmd,size);

    if (original.find(commands[6]) == 0)
        return new FgCommand(cmdLine,original,splitCmd,size);

    if (original.find(commands[7]) == 0)
        return new BgCommand(cmdLine,original,splitCmd,size);

    if (original.find(commands[8]) == 0)
        return new QuitCommand(cmdLine,original,splitCmd,size);

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
        splitCmd[i++] = string(str);
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

void SmallShell::cd(const string &str) {
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

pid_t SmallShell::getMyPid() const {
    return myPid;
}
