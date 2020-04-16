#ifndef OSHW1_COMMAND_H
#define OSHW1_COMMAND_H
#include "Libs.h"

class Command {
    string originalCommandLine;
    string args[COMMAND_MAX_ARGS];
    int size;
    cmdType type;
public:
    virtual void execute() = 0;
    const string print() const;

    //Wait for now
    virtual Command clone() = 0;

};


#endif //OSHW1_COMMAND_H
