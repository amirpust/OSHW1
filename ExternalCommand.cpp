#include "ExternalCommand.h"
#include <limits.h>

#define DEFAULT_BUFFER_SIZE 4096
void ExternalCommand::execute() {
    char bash[10] = "/bin/bash";
    char flag[3] = "-c";
    string s = "";

    for (int i = 0; i < size; ++i) {
        s += splitLine[i];
        s += " ";
    }
    char cmd[COMMAND_ARGS_MAX_LENGTH + 1];
    strcpy(cmd,s.c_str());
    char* cmd_args[COMMAND_MAX_ARGS + 4];
    cmd_args[0] = bash;
    cmd_args[1] = flag;
    cmd_args[2] = cmd;
    cmd_args[3] = NULL;

    if(execv(bash,cmd_args) == EXECV_ERR)
        throw execvError();
}

ExternalCommand::ExternalCommand(string *args, int size) :
Command(args,size,external){}

CopyCommand::CopyCommand(string *args, int size)
        : Command(args, size, copyCmd){
    oldFile = splitLine[1];
    newFile = splitLine[2];
}

CopyCommand::CopyCommand(CopyCommand const &toCopy)
        :Command(toCopy.splitLine, toCopy.size, copyCmd){
    oldFile = toCopy.oldFile;
    newFile = toCopy.newFile;
}

void CopyCommand::execute() {
    char realPath1[PATH_MAX];
    char realPath2[PATH_MAX];
    realpath(oldFile.c_str(), realPath1);
    realpath(newFile.c_str(), realPath2);

    if(strcmp(realPath1,realPath2) == 0){
        readFD = open(oldFile.c_str(),O_RDONLY);
        if(readFD == READ_ERR){
            throw openError();
        }
        close(readFD);
        cout << "smash: " + oldFile + " was copied to " + newFile << endl;
        return;
    }
    prepare();

    char buf[DEFAULT_BUFFER_SIZE];

    int readRet = read(readFD,buf,DEFAULT_BUFFER_SIZE);
    if(readRet == READ_ERR)
        throw readError();

    int writeRet = write(writeFD,buf,readRet);
    if(writeRet == WRITE_ERR)
        throw writeError();

    while(readRet > 0 && writeRet > 0){

        readRet = read(readFD,buf,DEFAULT_BUFFER_SIZE);
        if(readRet == READ_ERR)
            throw readError();
        writeRet = write(writeFD,buf,readRet);
        if(writeRet == WRITE_ERR)
            throw writeError();

    }
    cout << "smash: " + oldFile + " was copied to " + newFile << endl; // this should be printed but i forgot it
    cleanUp();
}

void CopyCommand::prepare() {
    readFD = open(oldFile.c_str(),O_RDONLY);
    if(readFD == READ_ERR){
        throw openError();
    }
    if(oldFile == newFile)
        return;
    writeFD = open(newFile.c_str(),O_CREAT | O_RDWR | O_TRUNC, S_IRWXU);
    if(writeFD == WRITE_ERR){
        throw openError();
    }
}

void CopyCommand::cleanUp() {
    if(readFD > -1)
        if(close(readFD) == CLOSE_ERR)
            throw closeError();
    if(writeFD > -1)
        if(close(writeFD) == CLOSE_ERR)
            throw closeError();
}

