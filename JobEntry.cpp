//
// Created by Amir on 16/04/2020.
//

#include "JobEntry.h"

JobEntry::JobEntry(const ExternalCommand &_cmd, int _jobId, pid_t p) :
        cmd(ExternalCommand(_cmd)), status(RUN), startTime(time(nullptr)), jobId(_jobId),pid(p){}

void JobEntry::updateStatus() {
    PRINT_START_PARAM(cmd->print());
    PRINT_PARAM(pid);

    if(status == END)
        return;

    int newStatus = 0;
    pid_t p = waitpid(pid, &newStatus, WNOHANG | WUNTRACED | WCONTINUED);

    /*cout << "Checking  " << p << endl;
    cout << "Checking status of " << pid << endl;
    cout << "newStatus: " << newStatus << endl;*/

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

    PRINT_END;
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

const string JobEntry::print() const{
    if(status == STOP)
        return cmd.print();
    return cmd.print() + " (stopped)";
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




