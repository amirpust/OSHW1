//
// Created by Amir on 16/04/2020.
//

#include "JobEntry.h"

JobEntry::JobEntry (const string& _originalCmd, int _jobId, pid_t _p, const string& _path) :
       originalcmd(string(_originalCmd)), status(RUN), startTime(time(nullptr)), jobId(_jobId),pid(_p), path(string(_path)){}


void JobEntry::updateStatus() {
    if(status == END)
        return;

    int newStatus = 0;

    pid_t p = waitpid(pid, &newStatus, WNOHANG | WUNTRACED | WCONTINUED);
    if (p == WAIT_PID_ERR)
        throw waitpidError();

    if( p != 0){
        if(WIFSTOPPED(newStatus)){
            stopTime = time(nullptr);
            status = STOP;
        }else if(WIFCONTINUED(newStatus)){
            startTime = time(nullptr);
            status = RUN;
        }else if(WIFEXITED(newStatus) || WIFSIGNALED(newStatus)){
            status = END;
        }else{
            status = RUN;
        }
    }
}

cmdStatus JobEntry::getStatus() const {
    return status;
}

int JobEntry::getJobId() const {
    return jobId;
}

pid_t JobEntry::getPid() const {
    return pid;
}

const string &JobEntry::getPath() const {
    return path;
}

time_t JobEntry::getTime() const {
    if (status == STOP)
        return difftime(stopTime,startTime);
    return difftime(time(nullptr),startTime);
}

const string& JobEntry::print() const{
    return originalcmd;
}

void JobEntry::stopCmd() {
    /*
    if(status != RUN)
        return;*/// TODO: check

    if (kill(pid, SIGSTOP) == KILL_ERR)
        throw killError();
    //updateStatus();
}

void JobEntry::continueCmd() {
    /*
    if(status != STOP)
        return;*/

    if(kill(pid, SIGCONT) == KILL_ERR)
        throw killError();
    //updateStatus();
}

void JobEntry::killCmd() {
    /*
    if(status == END)
        return;*/

    if(kill(pid, SIGKILL) == KILL_ERR)
        throw killError();

    int p = waitpid(pid, NULL, 0);
    status = END;

    //updateStatus();
}

JobEntry::JobEntry(const JobEntry &toCopy) : originalcmd(toCopy.originalcmd) , status(toCopy.status), startTime(toCopy.startTime), stopTime(toCopy.stopTime),
                                             jobId(toCopy.jobId), pid(toCopy.pid), path(toCopy.path){}


