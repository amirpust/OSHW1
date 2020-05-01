//
// Created by Amir on 16/04/2020.
//

#include "JobEntry.h"

JobEntry::JobEntry (const string& _originalCmd, int _jobId, const pid_t _p, const pid_t _pid2, time_t _timeOut) :
       originalcmd(string(_originalCmd)), status(RUN), status2(RUN), startTime(time(nullptr)), jobId(_jobId),pid(_p), pid2(_pid2), timeOut(time(&_timeOut)){
    if(pid2 == -1)
        status2 = END;
}


void JobEntry::updateStatus(bool inFG) {
    int newStatus = 0;
    pid_t p;
    if(status != END){
        if(inFG && status != STOP)
            p = waitpid(pid, &newStatus, WUNTRACED | WCONTINUED);
        else
            p = waitpid(pid, &newStatus, WNOHANG | WUNTRACED | WCONTINUED);

        /*
        if (p == WAIT_PID_ERR)
            throw waitpidError( __FUNCTION__, __LINE__);
        */
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

    if(status2 != END ){
        if(inFG && status != STOP)
            p = waitpid(pid2, &newStatus, WUNTRACED | WCONTINUED);
        else
            p = waitpid(pid2, &newStatus, WNOHANG | WUNTRACED | WCONTINUED);

        /*
        if (p == WAIT_PID_ERR)
            throw waitpidError(__FUNCTION__, __LINE__);
        */
        if( p != 0){
            if(WIFSTOPPED(newStatus)){
                stopTime = time(nullptr);
                status2 = STOP;
            }else if(WIFCONTINUED(newStatus)){
                startTime = time(nullptr);
                status2 = RUN;
            }else if(WIFEXITED(newStatus) || WIFSIGNALED(newStatus)){
                status2 = END;
            }else{
                status2 = RUN;
            }
        }
    }
}

cmdStatus JobEntry::getStatus() const {
    return status;
}

cmdStatus JobEntry::getStatus2() const {
    return status2;
}

int JobEntry::getJobId() const {
    return jobId;
}

pid_t JobEntry::getPid() const {
    return pid;
}

time_t JobEntry::getTime() const {
    if (status == STOP || status2 == STOP)
        return difftime(stopTime,startTime);
    return difftime(time(nullptr),startTime);
}

const string& JobEntry::print() const{
    return originalcmd;
}

void JobEntry::stopCmd() {
    if(status == RUN){
        if (kill(pid, SIGSTOP) == KILL_ERR)
            throw killError();
    }

    if(status2 == RUN){
        if (kill(pid2, SIGSTOP) == KILL_ERR)
            throw killError();
    }

}

void JobEntry::continueCmd() {
    if(status == STOP){
        if(kill(pid, SIGCONT) == KILL_ERR)
            throw killError();
    }

    if(status2 == STOP){
        if(kill(pid2, SIGCONT) == KILL_ERR)
            throw killError();
    }
    updateStatus();
}

void JobEntry::killCmd() {
    if(status != END){
        assert(pid > 0);
        if(kill(pid, SIGKILL) == KILL_ERR)
            throw killError();
        waitpid(pid, NULL, 0);
        status = END;
    }
    if(status2 != END){
        assert(pid2 > 0);
        if(kill(pid2, SIGKILL) == KILL_ERR)
            throw killError();
        waitpid(pid2, NULL, 0);
        status2 = END;
    }
}

JobEntry::JobEntry(const JobEntry &toCopy) : originalcmd(toCopy.originalcmd) , status(toCopy.status),status2(toCopy.status2), startTime(toCopy.startTime), stopTime(toCopy.stopTime),
                                             jobId(toCopy.jobId), pid(toCopy.pid), pid2(toCopy.pid2), timeOut(toCopy.timeOut){} //TODO

pid_t JobEntry::getPid2() const {
    return pid2;
}

time_t JobEntry::getTimeOut() const {
    return timeOut;
}


