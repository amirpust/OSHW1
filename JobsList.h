#ifndef OSHW1_JOBSLIST_H
#define OSHW1_JOBSLIST_H

#include "Libs.h"
#include "JobEntry.h"

class JobsList {
    int maxId;
    vector<JobEntry*> jobs;
    JobEntry* fg;

public:
    JobsList() : counter(0), maxId(0), jobs(), fg(NULL) {};
    ~JobsList();
    void addJob(Command* cmd,pid_t p, bool onBg);











    class notExist: public std::exception{};
    class emptyList: public std::exception{};
    class inBG: public std::exception{};
};


#endif //OSHW1_JOBSLIST_H
