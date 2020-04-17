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
    time_t startTime;
    time_t stopTime;
    int jobId;
    pid_t pid;
    string path;

public:

    JobEntry(const string& originalCmd, int _jobId, pid_t _p, const string& _path);
    JobEntry(const JobEntry& toCopy);
    void updateStatus();
    cmdStatus getStatus() const;
    int getJobId() const;
    pid_t getPid() const;
    const string& getPath() const;
    time_t getTime() const;
    const string& print() const;

    void stopCmd();
    void continueCmd();
    void killCmd();
};


#endif //OSHW1_JOBENTRY_H
