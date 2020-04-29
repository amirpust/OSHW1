#ifndef OSHW1_SYSCALLEXCEPTION_H
#define OSHW1_SYSCALLEXCEPTION_H

#include <exception>
#include <cstdio>
#include <cstring>


using namespace std;

class SysCallException : public exception{
    const char* func;
    const int line;
public:
    explicit SysCallException(const char* _func = "" , int _line = -1) : func(_func), line(_line){};
    virtual string print() = 0;
    string printDebug(){
        string temp = print();
        temp += " | Func: ";
        temp += func;
        temp += " in line: ";
        temp += to_string(line);
        return temp;
    }
};

#define OPEN_ERR -1
class openError : public SysCallException{
public:
    explicit openError(const char* _func = "" , int _line = -1) : SysCallException(_func,_line){};
    string print() override {
        return "open";
    }

};

#define CLOSE_ERR -1
class closeError : public SysCallException{
public:
    string print() override {
        return "close";
    }
};

#define DUP_ERR -1
class dupError : public SysCallException{
public:
    string print() override {
        return "dup";
    }
};

#define WRITE_ERR -1
class writeError : public SysCallException{
public:
    string print() override {
        return "write";
    }
};

#define READ_ERR -1
class readError : public SysCallException{
public:
    string print() override {
        return "read";
    }
};

#define FORK_ERR -1
class forkError : public SysCallException{
public:
    string print() override {
        return "fork";
    }
};

#define PIPE_ERR -1
class pipeError : public SysCallException{
public:
    string print() override {
        return "pipe";
    }
};

#define CHDIR_ERR -1
class chdirError : public SysCallException{
public:
    string print() override {
        return "chdir";
    }
};

#define EXECV_ERR -1
class execvError : public SysCallException{
public:
    string print() override {
        return "execv";
    }
};

#define GETPID_ERR -1
class getpidError : public SysCallException{
public:
    string print() override {
        return "getpid";
    }
};

#define GET_CURR_DIR_ERR NULL
class getCurrentDirError : public SysCallException{
public:
    string print() override {
        return "get_current_dir";
    }
};

#define KILL_ERR -1
class killError : public SysCallException{
public:
    string print() override {
        return "kill";
    }
};

#define WAIT_PID_ERR -1
class waitpidError : public SysCallException{
public:
    waitpidError(const char* _func = "", int _line = -1) : SysCallException(_func,_line) {}

    string print() override {
        return "waitpid";
    }
};

#define SETPGRP_ERR -1
class setpgrpError : public SysCallException{
public:
    string print() override {
        return "setpgrp";
    }
};

#endif //OSHW1_SYSCALLEXCEPTION_H
