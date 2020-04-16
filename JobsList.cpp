//
// Created by Amir on 16/04/2020.
//

#include "JobsList.h"

JobsList::JobsList() : maxId(0), jobs(), fg(nullptr) {}

void
JobsList::addJob(const ExternalCommand &command, pid_t pid, const string &path,
                 bool onBg) {
    update();
    jobs.emplace_back(command, ++maxId, pid, path);

    if(!onBg)
        bringFG(maxId);
}

void JobsList::printJobsList() {
    update();

    if (jobs.empty())
        return;

    for (const JobEntry& i : jobs){
        cout << "[" << i.getJobId() << "] " << i.print();
        cout << " : " << i.getPid() << " ";
        cout << i.getTime() <<" secs";
        if(i.getStatus() != RUN)
            cout << " (stopped)";
        cout << endl;
    }
}

void JobsList::killAllJobs() {
    for(auto i : jobs) {
        killJob(i);
    }
    update();
}

int JobsList::getSize() {
    return jobs.size();
}

void JobsList::sendSigById(int sig, int jobId) {
    JobEntry* job;
    if( jobId == 0){
        assert(fg != nullptr);
        if(!fg)
            return;
        job = fg;
    }
    else
        job = &getJobById(jobId);



    if(sig == SIGKILL){
        job->killCmd();
    }else if (sig == SIGSTOP){
        job->stopCmd();
    }else if(sig == SIGCONT){
        job->continueCmd();
    }else{
        kill(job->getPid(),sig);
    }
}

void JobsList::bringFG(int jobId) {
    assert((fg == nullptr));

    fg = &getJobById(jobId);

    if(fg->getStatus() == STOP)
        fg->continueCmd();

    update();
}

void JobsList::resumeOnBG(int jobId) {
    update();
    if(jobId == 0){
        getLastStoppedJob().continueCmd();
    }else{
        JobEntry& job = getJobById(jobId);
        if (job.getStatus() == RUN)
            throw inBG();

        job.continueCmd();
    }
}

void JobsList::update() {
    runFG();
    removeFinishedJobs();

    if(jobs.empty())
        maxId = 0;
    else
        maxId = jobs.back().getJobId();
}

void JobsList::runFG() {
    if (!fg)
        return;

    do {
        fg->updateStatus();
    } while (fg->getStatus() == RUN);

    fg = nullptr;
}

void JobsList::removeFinishedJobs() {
    if(jobs.empty())
        return;

    vector<JobEntry> temp;
    for(JobEntry& i : jobs){
        i.updateStatus();
        if(i.getStatus() != END)
            temp.push_back(i); //TODO: check if push_back really duplicate
    }

    jobs.clear();

    for(const JobEntry& i:temp)
        jobs.push_back(i);

}

void JobsList::printKilledCommand(JobEntry &job) {
    cout << job.getPid() << ": " << job.print() << endl;
}

void JobsList::killJob(JobEntry &job, bool toPrint) {
    if (toPrint)
        printKilledCommand(job);

    job.killCmd();
}

JobEntry &JobsList::getJobById(int jobId) {
    update();
    for (JobEntry& i : jobs)
        if(i.getJobId() == jobId)
            return i;

    throw notExist();
}

JobEntry &JobsList::getLastStoppedJob() {
    if(jobs.empty())
        throw emptyList();

    JobEntry* lastStopped = NULL;
    for(JobEntry& i : jobs){
        if (i.getStatus() == STOP)
            lastStopped = &i;
    }

    if(!lastStopped){
        throw notExist();
    }

    return (*lastStopped);
}

JobsList &JobsList::getInstance() {
    static JobsList instance;
    return instance;
}

pid_t JobsList::fgPid() {
    if(!fg)
        return -1;
    return fg->getPid();
}
