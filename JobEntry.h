#ifndef OSHW1_JOBENTRY_H
#define OSHW1_JOBENTRY_H

#include "Libs.h"
#include "ExternalCommand.h"

typedef enum{
    RUN, STOP, END
}cmdStatus;

class JobEntry {
    ExternalCommand command;
    cmdStatus status;
    time_t startTime;
    time_t stopTime;
    int jobId;
    pid_t pid;
    string path;

};


#endif //OSHW1_JOBENTRY_H
