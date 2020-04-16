#ifndef OSHW1_BUILTINCOMMAND_H
#define OSHW1_BUILTINCOMMAND_H

#include "Libs.h"
#include "Command.h"
#include "JobsList.h"


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
    CdCommand(string& decrypted,string &originalCommandLine, string *args, int size);;

    void execute() override;
};

class JobsCommand: public BuiltInCommand{
public:
    JobsCommand(string& decrypted,string &originalCommandLine, string *args, int size):
    BuiltInCommand(decrypted,originalCommandLine,args,size){};

    void execute() override {
        try{
            JobsList::getInstance().printJobsList(); //TODO: add the try catch policy
        }catch (JobsList::emptyList& e){
            throw emptyList(splitLine[0].c_str());
        }
    }
};


#endif //OSHW1_BUILTINCOMMAND_H
