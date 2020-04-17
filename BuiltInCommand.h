#ifndef OSHW1_BUILTINCOMMAND_H
#define OSHW1_BUILTINCOMMAND_H

#include "Libs.h"
#include "Command.h"
#include "JobsList.h"
#include "SmallShell.h"
#include "SysCallException.h"

class BuiltInCommand : public Command{
public:
    BuiltInCommand(string &originalCommandLine, string *args, int size):
    Command(originalCommandLine,args,size,builtIn){};
    virtual ~BuiltInCommand() = default;
};

class ChpromptCommand: public BuiltInCommand{
public:
    ChpromptCommand(string &originalCommandLine, string *args, int size);;

    void execute() override;
};

class ShowpidCommand : public BuiltInCommand{
public:
    ShowpidCommand(string &originalCommandLine, string *args, int size);;

    void execute() override;
};

class PwdCommand : public BuiltInCommand{
public:
    PwdCommand(string &originalCommandLine, string *args, int size);;

    void execute() override;
};

class CdCommand : public BuiltInCommand{
public:
    CdCommand(string &originalCommandLine, string *args, int size);

    void execute() override;
};

class JobsCommand: public BuiltInCommand{
public:
    JobsCommand(string &originalCommandLine, string *args, int size);

    void execute() override;
};

class KillCommand : public BuiltInCommand{
    int sig;
    int jobId;
public:
    KillCommand(string &originalCommandLine, string *args, int size);

    void execute() override;
};


class FgCommand : public BuiltInCommand{
    int jobId;
public:
    FgCommand(string &originalCommandLine, string *args, int size);;
    void execute() override;

};

class BgCommand : public  BuiltInCommand{
    int jobId;
public:
    BgCommand(string &originalCommandLine, string *args, int size);

    void execute() override;
};


class QuitCommand : public BuiltInCommand{
public:
    QuitCommand(string &originalCommandLine, string *args, int size);
    void execute() override;
};




#endif //OSHW1_BUILTINCOMMAND_H
