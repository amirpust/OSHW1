#include "ExternalCommand.h"

void ExternalCommand::execute() {
    char bash[10] = "/bin/bash";
    char flag[3] = "-c";
    char cmd[COMMAND_ARGS_MAX_LENGTH + 1];
    strcpy(cmd,decryptedCmd.c_str());
    char* cmd_args[4];
    cmd_args[0] = bash;
    cmd_args[1] = flag;
    cmd_args[2] = cmd;
    cmd_args[3] = NULL;

    if(execv("/bin/bash",cmd_args) == EXECV_ERR)
        throw execvError();
}

ExternalCommand::ExternalCommand(string& decrypted,string &originalCommandLine, string *args, int size) :
Command(decrypted,originalCommandLine,args,size,external){}

CopyCommand::CopyCommand(string &decrypted, string &originalCommandLine, string *args, int size)
        : Command(decrypted, originalCommandLine, args, size, copyCmd){
    //TODO: check arguments
    oldFile = splitLine[1];
    newFile = splitLine[2];
}

CopyCommand::CopyCommand(CopyCommand const &toCopy)
        :Command(toCopy.decryptedCmd, toCopy.originalCommandLine, toCopy.splitLine, toCopy.size, copyCmd){
    oldFile = toCopy.oldFile;
    newFile = toCopy.newFile;
}

void CopyCommand::execute() {
    prepare();
    char buf[1];

    int readRet = read(readFD,buf,1);
    if(readRet == READ_ERR)
        throw readError();

    int writeRet = write(writeFD,buf,1);
    if(writeRet == WRITE_ERR)
        throw writeError();

    while(readRet > 0 && writeRet > 0){

        readRet = read(readFD,buf,1);
        if(readRet == READ_ERR)
            throw readError();

        writeRet = write(writeFD,buf,1);
        if(writeRet == WRITE_ERR)
            throw writeError();
    }
    cleanUp();
}

void CopyCommand::prepare() {
    readFD = open(oldFile.c_str(),O_RDONLY);
    writeFD = open(newFile.c_str(),O_CREAT | O_RDWR | O_TRUNC, S_IRWXU);
    if(readFD == READ_ERR || writeFD == WRITE_ERR){
        throw openError(__FUNCTION__,__LINE__);
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
