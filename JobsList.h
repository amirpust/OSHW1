#ifndef OSHW1_JOBSLIST_H
#define OSHW1_JOBSLIST_H

#include "Libs.h"
#include "JobEntry.h"

class JobsList {
    int maxId;
    vector<JobEntry> jobs;
    JobEntry* fg;
    JobsList();

public:
    JobsList(const JobsList&) = delete;
    JobsList& operator=(const JobsList&) = delete;
    static JobsList& getInstance();

    ~JobsList() = default;
    void addJob(const ExternalCommand& command,pid_t pid, const string& path, bool onBg);
    void printJobsList();
    void killAllJobs();
    int getSize();
    void sendSigById(int sig, int jobId = 0);
    void bringFG(int jobId);
    void resumeOnBG(int jobId = 0);

private:

    void update();
    void runFG();
    void removeFinishedJobs();
    void printKilledCommand(JobEntry& job);
    void killJob(JobEntry& job, bool toPrint = true);
    JobEntry& getJobById(int jobId);
    JobEntry& getLastStoppedJob();

public:

    class notExist: public std::exception{};
    class emptyList: public std::exception{};
    class inBG: public std::exception{};
};


#endif //OSHW1_JOBSLIST_H
