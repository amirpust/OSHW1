#include "SmallShell.h"
#include "BuiltInCommand.h"

#define TIMEOUT_ARGS 2
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
    pid_t pipeLeft = -1;
    pid_t pipeRight = -1;

    cmdDecryptor(original, &cmdStr, splitCmd, &size, &bg);

    time_t finishTime = 0;
    time_t startTime;
    bool timeout = isTimeout(splitCmd);
    if(timeout){
        startTime = time(nullptr);
        try {
            size_t index = 0;
            unsigned int duration = stoul(splitCmd[1], &index); //seconds sent and makes turns it into int

            if(index < splitCmd[1].size())
                throw invalidArgs(splitCmd[0].c_str());
        }catch(exception& e){
            throw invalidArgs(splitCmd[0].c_str()); //TODO : i dont know if its right to put it here
        }

        unsigned int duration = stoul(splitCmd[1]);
        finishTime = startTime + duration;
        alarm(duration); //TODO if duration is 0
        size -= TIMEOUT_ARGS;
        for (int i = 0; i < size; ++i) {
            splitCmd[i] = splitCmd[i + TIMEOUT_ARGS];
        }
    }

    int index = checkPipe(splitCmd, size, &pipeLeft, &pipeRight);
    if (index >= 0) {
        if (pipeLeft > 0 && pipeRight > 0) { //father
            jobs.addJob(original, bg, pipeLeft, pipeRight, finishTime); //TODO
            return;
        }else if(pipeLeft == 0){    //left son
            size = index;
        } else {                            //right son
            size -= (index + 1);
            for (int i = 0; i < size; i++)
                splitCmd[i] = splitCmd[i + index + 1];
        }
    }

    redirectionType io = identifyRedirection(splitCmd, size, &path);
    removeRedirection(cmdStr);
    prepareIO(io, path);

    Command *cmd = createCommand(splitCmd, size);


    if (cmd->getType() == builtIn) {
        cmd->execute();
        delete cmd;
    }else if (cmd->getType() == copyCmd) {
        CopyCommand copyCommand(*dynamic_cast<CopyCommand*>(cmd));
        delete cmd;

        if(pipeLeft == 0 || pipeRight == 0){
            copyCommand.execute();
            exit(-1);
        }

        pid_t childPid = fork();
        if (childPid == FORK_ERR){
            throw forkError();
        }

        if (childPid == 0) { //Child
            if (setpgrp() == SETPGRP_ERR)
                throw setpgrpError();
            copyCommand.execute();
            exit(-1);
        }else{
            jobs.addJob(original, bg, childPid,-1,finishTime);
        }
    }else {
        ExternalCommand externalCommand(*dynamic_cast<ExternalCommand *>(cmd));
        delete cmd;

        if(pipeLeft == 0 || pipeRight == 0){
            externalCommand.execute();
            exit(-1);
        }
        pid_t childPid = fork();
        if(childPid == FORK_ERR)
            throw forkError();
        if (childPid == 0) { //Child

            if (setpgrp() == SETPGRP_ERR)
                throw setpgrpError();

            externalCommand.execute();
            exit(-1);
        } else {
            jobs.addJob(original, bg, childPid, -1, finishTime);
        }
    }
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
        if(chdir(str.c_str()) == CHDIR_ERR) {
            throw chdirError();
        }
        char* temp = get_current_dir_name();
        if(temp == GET_CURR_DIR_ERR) {
            throw getCurrentDirError();
        }
        previousDir = currentDir;
        currentDir = string(temp);
        free(temp);
    }
}

pid_t SmallShell::getMyPid() const {
    return myPid;
}

Command* SmallShell::createCommand(string *splitCmd,int size) {
    const std::string commands [] = {
            "chprompt", "showpid", "pwd", "cd", "jobs", "kill", "fg",
            "bg", "quit" , "cp"
    };

    if (splitCmd[0] == commands[0])
        return new ChpromptCommand(splitCmd,size);

    if (splitCmd[0] == commands[1])
        return new ShowpidCommand(splitCmd,size);

    if (splitCmd[0] == commands[2])
        return new PwdCommand(splitCmd,size);

    if (splitCmd[0] == commands[3])
        return new CdCommand(splitCmd,size);

    if (splitCmd[0] == commands[4])
        return new JobsCommand(splitCmd,size);

    if (splitCmd[0] == commands[5])
        return new KillCommand(splitCmd,size);

    if (splitCmd[0] == commands[6])
        return new FgCommand(splitCmd,size);

    if (splitCmd[0] == commands[7])
        return new BgCommand(splitCmd,size);

    if (splitCmd[0] == commands[8])
        return new QuitCommand(splitCmd,size);

    if (splitCmd[0] == commands[9])
        return new CopyCommand(splitCmd,size);

    return new ExternalCommand(splitCmd,size);
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

void SmallShell::cleanUpIO() {
    close(0);
    close(1);
    close(2);

    dup(stdIn);
    dup(stdOut);
    dup(stdErr);
}

int SmallShell::checkPipe(string *split, int size, pid_t *leftPid, pid_t *rightPid) {

    for(int i = 0; i < size; i++){
        if(split[i]  == "|"){
            splitPipe(pipeRegular, leftPid, rightPid);
            return i;

        }else if(split[i] == "|&"){
            splitPipe(pipeStderr, leftPid, rightPid);
            return i;
        }
    }
    return -1;
}

void SmallShell::splitPipe(pipeType type, pid_t *leftPid, pid_t *rightPid) {
    fileDescriptor pipeFD[2];
    if (pipe(pipeFD) == PIPE_ERR)
       throw pipeError();

    (*leftPid) = fork();
    if((*leftPid) == FORK_ERR)
        throw forkError();

    if((*leftPid) == 0){        //Left son
        if (setpgrp() == SETPGRP_ERR)
            throw setpgrpError();
        (*rightPid) = -1;
        if(type == pipeRegular){
            if (close(1) == CLOSE_ERR)
                throw closeError();
        }else{
            if (close(2) == CLOSE_ERR)
                throw closeError();
        }
        if ( dup(pipeFD[1]) == DUP_ERR)
            throw dupError();

        if(close(pipeFD[0]) == CLOSE_ERR || close(pipeFD[1]) == CLOSE_ERR)
            throw closeError();

        return;
    }

    (*rightPid) = fork();
    if((*rightPid) == FORK_ERR)
        throw forkError();

    if((*rightPid) == 0){        //Right son
        if (setpgrp() == SETPGRP_ERR)
            throw setpgrpError();
        (*leftPid) = -1;
        if( close(0) == CLOSE_ERR)
            throw closeError();
        if( dup(pipeFD[0]) == DUP_ERR)
            throw dupError();

        if(close(pipeFD[0]) == CLOSE_ERR || close(pipeFD[1]) == CLOSE_ERR)
            throw closeError();

        return;
    }

    //Father
    if(close(pipeFD[0]) == CLOSE_ERR || close(pipeFD[1]) == CLOSE_ERR)
        throw closeError();
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

string SmallShell::cmdToString(const char *cmdLine) {
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
        }else if(WHITESPACE.find(cmdLine[i]) != string::npos){
            newCmd += " ";
        }else{
            newCmd += cmdLine[i];
        }
    }
    return newCmd;
}

bool SmallShell::isTimeout(string* splitCmd) {
    return splitCmd[0] == "timeout";
}