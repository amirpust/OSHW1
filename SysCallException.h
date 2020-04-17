#ifndef OSHW1_SYSCALLEXCEPTION_H
#define OSHW1_SYSCALLEXCEPTION_H

#include <exception>
#include <cstdio>
#include <cstring>

using namespace std;

class SysCallException : public exception{
public:
    virtual string print() = 0;
};

class openError : public SysCallException{
public:
    string print() override {
        return "open";
    }
};

class closeError : public SysCallException{
public:
    string print() override {
        return "close";
    }
};

class dupError : public SysCallException{
public:
    string print() override {
        return "dup";
    }
};

class writeError : public SysCallException{
public:
    string print() override {
        return "write";
    }
};

class readError : public SysCallException{
public:
    string print() override {
        return "read";
    }
};

class forkError : public SysCallException{
public:
    string print() override {
        return "fork";
    }
};

class pipeError : public SysCallException{
public:
    string print() override {
        return "pipe";
    }
};

class chdirError : public SysCallException{
public:
    string print() override {
        return "chdir";
    }
};

class execvError : public SysCallException{
public:
    string print() override {
        return "execv";
    }
};

class getpidError : public SysCallException{
public:
    string print() override {
        return "getpid";
    }
};

class getCurrentDirError : public SysCallException{
public:
    string print() override {
        return "get_current_dir";
    }
};
#endif //OSHW1_SYSCALLEXCEPTION_H
