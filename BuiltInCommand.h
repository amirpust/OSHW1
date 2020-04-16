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


class JobsCommand: public BuiltInCommand{
public:
    JobsCommand(string& decrypted,string &originalCommandLine, string *args, int size):
    BuiltInCommand(decrypted,originalCommandLine,args,size){};

    void execute() override {
        JobsList::getInstance().printJobsList(); //TODO: add the try catch policy
    }
};


#endif //OSHW1_BUILTINCOMMAND_H
