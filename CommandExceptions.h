
#ifndef HW1_COMMANDEXCEPTIONS_H
#define HW1_COMMANDEXCEPTIONS_H

//#include "Commands.h"
#include <exception>
#include <iostream>


using std::exception;
using std::string;

class commandException: public exception {
protected:
    const char* cmd;

public:

    virtual string print() const= 0;
    commandException(const char *cmd) : cmd(cmd) {
    }


};


class invalidArgs : public commandException{

public:
    explicit invalidArgs(const char* cmd) : commandException(cmd){};
    string print() const override  {
        return  " error: " + string(cmd) + ": invalid arguments";
    }
};

class tooManyArgs : public commandException{
public:
    explicit tooManyArgs(const char* cmd): commandException(cmd){};
    string print() const override {
        return  " error: " + string(cmd) + ":  too many arguments";
    }
};

class emptyDirHistory : public commandException{
public:
    explicit emptyDirHistory(const char* cmd) : commandException(cmd){};
protected:
    string print() const override {
        return " error: " + string(cmd) + ":  OLDPWD not set";
    }
};



class jobDoesntExist : public commandException{
    int jobId;

public:

    jobDoesntExist(const char* _cmd, int _jobId): commandException(_cmd), jobId(_jobId){};
    string print() const override {
        return  " error: " + string(cmd) + ": " + std::to_string(jobId) + " does not exist";
    }
};


class jobAlreadyBGRuning: public commandException{
    int jobId;
public:
    string print() const override {
        return " error: " + string(cmd) + ": " + std::to_string(jobId)
               + " is already running in the background";
    }

    jobAlreadyBGRuning(const char *cmd, int id) : commandException(cmd), jobId(id) {

    }

};

class emptyListBG : public  commandException{

public:
    emptyListBG(const char* cmd):commandException(cmd){};

    string print() const override {
        return " error: " + string(cmd) + ": "
               + " there is no stopped jobs to resume";
    }
};

class emptyList : public commandException{
public:
    string print() const override {
        return " error: " + string(cmd) + ": jobs list is empty";
    }

    emptyList(const char *cmd) : commandException(cmd) {

    }
};
#endif //HW1_COMMANDEXCEPTIONS_H
