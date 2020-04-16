#ifndef OSHW1_COMMAND_H
#define OSHW1_COMMAND_H
#include "Libs.h"


#define COMMAND_ARGS_MAX_LENGTH (200)
#define COMMAND_MAX_ARGS (20)
#define HISTORY_MAX_RECORDS (50)
typedef enum {builtIn, external, pipeCmd, redirection} cmdType;
typedef enum{override, append, noRedirect} redirectionType;
typedef  enum{pipeRegular,pipeStderr,noPipe}pipeType;



class Command {
protected:
    string decryptedCmd;//only the command itself
    string originalCommandLine; // the full command
    string splitLine[COMMAND_MAX_ARGS]; // cmd after splitting
    int size; // size of the array
    cmdType type; // the type of the command
public:

    Command(const string& cmdLine,const string &originalCommandLine,const string *args,int size,cmdType type);

    virtual ~Command() = default;

    virtual void execute() = 0;
    const string print() const{
        return originalCommandLine;
    };

    cmdType getType() const;

};


#endif //OSHW1_COMMAND_H
