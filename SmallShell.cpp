#include "SmallShell.h"
#include "BuiltInCommand.h"


SmallShell &SmallShell::getInstance() {
    static SmallShell instance; // Guaranteed to be destroyed.
    // Instantiated on first use.
    return instance;
}

void SmallShell::executeCommand(const char *cmd_line) {
    bool bg;
    string original = cmdToString(cmd_line);
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
    removeRedirection(cmdStr);
    prepareIO(io, path);
    Command *cmd = createCommand(original, cmdStr, splitCmd, size);


    if (cmd->getType() == builtIn) {
        cmd->execute();
        delete cmd;
    }else if (cmd->getType() == copyCmd) {
        pid_t childPid = fork();
        if (childPid == FORK_ERR){
            delete cmd;
            throw forkError();
        }
        CopyCommand copyCommand(*dynamic_cast<CopyCommand*>(cmd));
        delete cmd;
        if (childPid == 0) { //Child
            if (setpgrp() == SETPGRP_ERR)
                throw setpgrpError();
            copyCommand.execute();
            exit(-1);
        }else{
            jobs.addJob(copyCommand.print(), childPid, "./", bg);
        }
    }else {
        pid_t childPid = fork();
        ExternalCommand externalCommand(*dynamic_cast<ExternalCommand *>(cmd));
        delete cmd;
        if(childPid == FORK_ERR)
            throw forkError();
        if (childPid == 0) { //Child

            if (setpgrp() == SETPGRP_ERR)
                throw setpgrpError();

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
        if(chdir(previousDir.c_str()) == CHDIR_ERR)
            throw chdirError();
        string temp = currentDir;
        currentDir = previousDir;
        previousDir = temp;
    }else{
        previousDir = currentDir;
        if(chdir(str.c_str()) == CHDIR_ERR)
            throw chdirError();
        char* temp = get_current_dir_name();
        if(temp == GET_CURR_DIR_ERR)
            throw getCurrentDirError();

        currentDir = string(temp);
        free(temp);
    }
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
        return new ChpromptCommand(original,splitCmd,size);

    if (original.find(commands[1]) == 0)
        return new ShowpidCommand(original,splitCmd,size);

    if (original.find(commands[2]) == 0)
        return new PwdCommand(original,splitCmd,size);

    if (original.find(commands[3]) == 0)
        return new CdCommand(original,splitCmd,size);

    if (original.find(commands[4]) == 0)
        return new JobsCommand(original,splitCmd,size);

    if (original.find(commands[5]) == 0)
        return new KillCommand(original,splitCmd,size);

    if (original.find(commands[6]) == 0)
        return new FgCommand(original,splitCmd,size);

    if (original.find(commands[7]) == 0)
        return new BgCommand(original,splitCmd,size);

    if (original.find(commands[8]) == 0)
        return new QuitCommand(original,splitCmd,size);
    if (original.find(commands[9]) == 0)
        return new CopyCommand(original,splitCmd,size);

    return new ExternalCommand(cmdLine,original,splitCmd,size);
}

const string &SmallShell::getName() const {
    return name;
}

SmallShell::SmallShell() : jobs(JobsList::getInstance()), defaultName("smash")
, name(defaultName),previousDir(""), running(true){
    char* temp = get_current_dir_name();
    if(temp == GET_CURR_DIR_ERR)
        throw getCurrentDirError();


    currentDir = string(temp);
    free(temp);

    myPid = getpid();
    stdIn = dup(0);
    stdOut = dup(1);
    stdErr = dup(2);

    if(stdIn == DUP_ERR || stdOut == DUP_ERR || stdErr == DUP_ERR)
        throw dupError();
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
        if(WHITESPACE.find(str) != string::npos)
            continue;
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

void SmallShell::prepareIO(redirectionType type, const string& path) {
    if(type == noRedirect)
        return;

    if (close(1) == CLOSE_ERR)
        throw closeError();

    if(type == override){
        if ( open(path.c_str(),O_CREAT | O_RDWR | O_TRUNC, S_IRWXU | S_IRGRP | S_IROTH) == OPEN_ERR)
            throw openError(__FUNCTION__,__LINE__);
    }else{
        if ( open(path.c_str(), O_APPEND | O_RDWR | O_CREAT, S_IRWXU | S_IRGRP | S_IROTH ) == OPEN_ERR)
            throw openError(__FUNCTION__,__LINE__);
    }
}

void SmallShell::cleanUpIO(pid_t pipePid) {
    if(pipePid == 0)
        exit(0);
    if(pipePid > 0){
        int status;
        if ( waitpid(pipePid, &status, 0) == WAIT_PID_ERR)
            throw waitpidError();
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
    if (pipe(pipeFD) == PIPE_ERR)
       throw pipeError();

    (*enterPid) = fork();

    if((*enterPid) == FORK_ERR)
        throw forkError();

    if ((*enterPid) == 0){                               //Enter (write to pipe)
        if(type == pipeRegular){
            if (close(1) == CLOSE_ERR)
                throw closeError();
        }else{
            if (close(2) == CLOSE_ERR)
                throw closeError();
        }
        if ( dup(pipeFD[1]) == DUP_ERR)
            throw dupError();



    }else{                                              //Exit (read from pipe)
        if( close(0) == CLOSE_ERR)
            throw closeError();
        if( dup(pipeFD[0]) == DUP_ERR)
            throw dupError();
    }
}

SmallShell::~SmallShell() {
    close(stdIn);
    close(stdOut);
    close(stdErr);
}

void SmallShell::removeRedirection(string &cmd) {
    int index = static_cast<int>(cmd.find_first_of('>'));
    if(index == string::npos)
        return;

    cmd[index] = '\0';
}

bool SmallShell::isRunning() const {
    return running;
}

void SmallShell::setRunning(bool running) {
    SmallShell::running = running;
}
