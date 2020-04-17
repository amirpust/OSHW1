#include "SmallShell.h"
#include "BuiltInCommand.h"


SmallShell &SmallShell::getInstance() {
    static SmallShell instance; // Guaranteed to be destroyed.
    // Instantiated on first use.
    return instance;
}

void SmallShell::executeCommand(const char *cmd_line) {
    bool bg;
    string original(cmd_line);
    string cmdStr;
    string splitCmd[COMMAND_MAX_ARGS];
    string path;
    int size = 0;
    pid_t pipePid = -1;

    cmdDecryptor(original, &cmdStr, splitCmd, &size, &bg);

    int index = checkPipe(splitCmd, size, &pipePid);
    if (index >= 0) {
        if (pipePid == 0) {
            size = index;
        } else {
            size -= (index + 1);
            for (int i = 0; i < size; i++)
                splitCmd[i] = splitCmd[i + index + 1];
        }
    }


    redirectionType io = identifyRedirection(splitCmd, size, &path);
    prepareIO(io, path);
    Command *cmd = createCommand(original, cmdStr, splitCmd, size);


    if (cmd->getType() == builtIn) {
        cmd->execute(); // TODO : delete this cmd also after execute
        delete cmd;
    }else if (cmd->getType() == copyCmd) {
        pid_t childPid = fork();
        CopyCommand copyCommand(*dynamic_cast<CopyCommand*>(cmd));
        delete cmd;
        if (childPid == 0) { //Child
            setpgrp();
            copyCommand.execute();
            exit(-1);
        }else{
            jobs.addJob(copyCommand.print(), childPid, "./", bg);
        }
    }else {
            pid_t childPid = fork();
            ExternalCommand externalCommand(*dynamic_cast<ExternalCommand *>(cmd));
            delete cmd;
            if (childPid == 0) { //Child
                setpgrp();
                externalCommand.execute();
                exit(-1);
            } else {
                jobs.addJob(externalCommand.print(), childPid, "./", bg);
            }
        }
        cleanUpIO(pipePid);
    }

void SmallShell::setName(const string &newName) {
    if(newName.empty())
        name = defaultName;
    else
        name = newName;
}

const string &SmallShell::getPreviousDir() const {
    return previousDir;
}

void SmallShell::cd(const string &str) {
    if(str == "-"){
        chdir(previousDir.c_str());
        string temp = currentDir;
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

Command* SmallShell::createCommand(string &original, string &cmdLine, string *splitCmd,
                          int size) {
    const std::string commands [] = {
            "chprompt", "showpid", "pwd", "cd", "jobs", "kill", "fg",
            "bg", "quit" , "cp"
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
    if (original.find(commands[9]) == 0)
        return new CopyCommand(cmdLine,original,splitCmd,size);

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
    stdIn = dup(0);
    stdOut = dup(1);
    stdErr = dup(2);
}

void SmallShell::cmdDecryptor(const string &original, string *cmd, string *splitCmd,
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
    int index = static_cast<int>(cmd.find_last_not_of(WHITESPACE));
    if(cmd[index] != '&')
        return;

    cmd[index] = '\0';
}

redirectionType SmallShell::identifyRedirection(string* splitCmd, int size, string* path){
    assert (path != nullptr);

    for(int i = 0; i < size - 1; i++){
        if(splitCmd[i] == ">"){
            *path = splitCmd[i+1];
            return override;
        }else if(splitCmd[i] == ">>"){
            *path = splitCmd[i+1];
            return append;
        }
    }
    return noRedirect;
}

void SmallShell::prepareIO(redirectionType type, string path) {
    if(type == noRedirect)
        return;

    close(1);
    if(type == override){
        open(path.c_str(),O_CREAT | O_RDWR, S_IRWXU | S_IRGRP | S_IROTH);
    }else{
        open(path.c_str(), O_APPEND | O_RDWR | O_CREAT, S_IRWXU | S_IRGRP | S_IROTH );
    }
}

void SmallShell::cleanUpIO(pid_t pipePid) {
    if(pipePid == 0)
        exit(0);
    if(pipePid > 0){
        int status;
        waitpid(pipePid, &status);
    }

    close(0);
    close(1);
    close(2);

    dup(stdIn);
    dup(stdOut);
    dup(stdErr);
}

int SmallShell::checkPipe(string *split, int size, pid_t *enterPid) {

    for(int i = 0; i < size; i++){
        if(split[i]  == "|"){
            splitPipe(pipeRegular, enterPid);
            return i;

        }else if(split[i] == "|&"){
            splitPipe(pipeStderr, enterPid);
            return i;
        }
    }
    return -1;
}

void SmallShell::splitPipe(pipeType type, pid_t *enterPid) {
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
}

SmallShell::~SmallShell() {
    close(stdIn);
    close(stdOut);
    close(stdErr);
}
