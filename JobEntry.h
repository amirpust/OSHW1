#ifndef OSHW1_JOBENTRY_H
#define OSHW1_JOBENTRY_H

#include "Libs.h"
#include "ExternalCommand.h"

typedef enum{
    RUN, STOP, END
}cmdStatus;

class JobEntry {
    const string originalcmd;
    cmdStatus status;
    cmdStatus status2;
    time_t startTime;
    time_t stopTime;
    int jobId;
    pid_t pid;
    pid_t pid2;

public:

    JobEntry(const string& originalCmd, int _jobId, pid_t _p, pid_t _pid2 = -1);
    JobEntry(const JobEntry& toCopy);
    void updateStatus(bool inFG = false);
    cmdStatus getStatus() const;
    cmdStatus getStatus2() const;
    int getJobId() const;
    pid_t getPid() const;
    time_t getTime() const;
    const string& print() const;

    pid_t getPid2() const;

    void stopCmd();
    void continueCmd();
    void killCmd();
};


#endif //OSHW1_JOBENTRY_H
