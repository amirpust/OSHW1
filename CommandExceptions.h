
#ifndef HW1_COMMANDEXCEPTIONS_H
#define HW1_COMMANDEXCEPTIONS_H

//#include "Commands.h"
#include <exception>
#include <iostream>



using std::exception;
using std::string;

class commandException: public exception {
protected:
    string cmd;

public:

    virtual string print() const= 0;
    commandException(string cmd) : cmd(string(cmd)) {
    }


};

class invalidArgs : public commandException{

public:
    explicit invalidArgs(string cmd) : commandException(cmd){};
    string print() const override  {
        return  " error: " + cmd + ": invalid arguments";
    }
};

class tooManyArgs : public commandException{
public:
    explicit tooManyArgs(string cmd): commandException(cmd){};
    string print() const override {
        return  " error: " + cmd +": too many arguments";
    }
};

class emptyDirHistory : public commandException{
public:
    explicit emptyDirHistory(string cmd) : commandException(cmd){};
protected:
    string print() const override {
        return " error: " + cmd +": OLDPWD not set";
    }
};

class jobDoesntExist : public commandException{
    int jobId;

public:

    jobDoesntExist(string _cmd, int _jobId): commandException(_cmd), jobId(_jobId){};
    string print() const override {
        return  " error: " + cmd +": job-id " + std::to_string(jobId) + " does not exist";
    }
};


class jobAlreadyBGRuning: public commandException{
    int jobId;
public:
    string print() const override {
        return " error: " + cmd +": job-id " + std::to_string(jobId)
               + " is already running in the background";
    }

    jobAlreadyBGRuning(string cmd, int id) : commandException(cmd), jobId(id) {

    }

};

class emptyListBG : public  commandException{

public:
    emptyListBG(string cmd):commandException(cmd){};

    string print() const override {
        return " error: " + cmd +": there is no stopped jobs to resume";
    }
};

class emptyList : public commandException{
public:
    string print() const override {
        return " error: " + cmd +": jobs list is empty";
    }

    emptyList(string cmd) : commandException(cmd) {

    }
};
#endif //HW1_COMMANDEXCEPTIONS_H
