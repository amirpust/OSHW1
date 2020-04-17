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

    if( p != 0){
        if(WIFSTOPPED(newStatus)){
            //TODO: debug
            cout << "update status stop" << endl;
            stopTime = time(nullptr);
            status = STOP;
        }else if(WIFCONTINUED(newStatus)){
            cout << "update status continued" << endl;
            startTime = time(nullptr);
            status = RUN;
        }else if(WIFEXITED(newStatus) || WTERMSIG(newStatus)){
            cout << "update status end" << endl;
            status = END;
        }else{
            cout << "Error: else | Function: " << __FUNCTION__ << " | Line: " << __LINE__  << endl;
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
    assert(status == RUN); // TODO: debug
    if(status != RUN)
        return;

    kill(pid, SIGSTOP);
    updateStatus();
}

void JobEntry::continueCmd() {
    assert(status == STOP); //TODO: debug
    if(status != STOP)
        return;

    kill(pid, SIGCONT); //TODO:Check
    updateStatus();
}

void JobEntry::killCmd() {
    assert(status != END); //TODO: debug
    if(status == END)
        return;

    kill(pid, SIGKILL);
    updateStatus();
}

JobEntry::JobEntry(const JobEntry &toCopy) : originalcmd(toCopy.originalcmd) , status(toCopy.status), startTime(toCopy.startTime), stopTime(toCopy.stopTime),
                                             jobId(toCopy.jobId), pid(toCopy.pid), path(toCopy.path){}


