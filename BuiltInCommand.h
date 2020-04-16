#ifndef OSHW1_BUILTINCOMMAND_H
#define OSHW1_BUILTINCOMMAND_H

#include "Libs.h"
#include "Command.h"
#include "JobsList.h"
#include "SmallShell.h"


class BuiltInCommand : public Command{
public:
    BuiltInCommand(string& decrypted,string &originalCommandLine, string *args, int size):
    Command(decrypted,originalCommandLine,args,size,builtIn){};
    virtual ~BuiltInCommand() = default;
};

class ChpromptCommand: public BuiltInCommand{
public:
    ChpromptCommand(string& decrypted,string &originalCommandLine, string *args, int size);;

    void execute() override;
};

class ShowpidCommand : public BuiltInCommand{
public:
    ShowpidCommand(string& decrypted,string &originalCommandLine, string *args, int size);;

    void execute() override;
};

class PwdCommand : public BuiltInCommand{
public:
    PwdCommand(string& decrypted,string &originalCommandLine, string *args, int size);;

    void execute() override;
};

class CdCommand : public BuiltInCommand{
public:
    CdCommand(string& decrypted,string &originalCommandLine, string *args, int size);

    void execute() override;
};

class JobsCommand: public BuiltInCommand{
public:
    JobsCommand(string& decrypted,string &originalCommandLine, string *args, int size);

    void execute() override;
};

class KillCommand : public BuiltInCommand{
    int sig;
    int jobId;
public:
    KillCommand(string& decrypted,string &originalCommandLine, string *args, int size);

    void execute() override;
};


class FgCommand : public BuiltInCommand{
    int jobId;
public:
    FgCommand(string& decrypted,string &originalCommandLine, string *args, int size);;
    void execute() override;

};

class BgCommand : public  BuiltInCommand{
    int jobId;
public:
    BgCommand(string& decrypted,string &originalCommandLine, string *args, int size);

    void execute() override;
};


class QuitCommand : public BuiltInCommand{
public:
    QuitCommand(string& decrypted,string &originalCommandLine, string *args, int size);
    void execute() override;
};





class CopyCommand : public Command{
    string oldFile;
    string newFile;
    int readFD;
    int writeFD;


public:

    CopyCommand(string& decrypted,string &originalCommandLine, string *args, int size)
    : Command(decrypted, originalCommandLine, args, size, copyCmd){
        //TODO: check arguments
        oldFile = splitLine[1];
        newFile = splitLine[2];
    }

    void execute()override {
        prepare();
        char buf[1];
        while(read(readFD,buf,1) > 0 && write(writeFD,buf,1) > 0){}
    }

private:
    void prepare(){
        //opening reading file
        readFD = open(oldFile.c_str(),O_RDONLY);
        if(readFD == -1){
            //TODO: should return some kind of error
        }
        writeFD = open(newFile.c_str(),O_CREAT | O_RDWR | O_TRUNC, S_IWUSR);
        if (writeFD == -1){
            //TODO: should return some kind of error
        }
    }

};

#endif //OSHW1_BUILTINCOMMAND_H
