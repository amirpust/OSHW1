//
// Created by Amir on 16/04/2020.
//

#include "JobsList.h"

JobsList::JobsList() : maxId(0), jobs(), fg(nullptr) {}

void JobsList::addJob(const string& originalCmd,  bool onBg, pid_t pid, pid_t pid2, time_t endTime) {
    update();
    int newId = ++maxId;
    jobs.emplace_back(originalCmd, newId, pid, pid2, endTime);

    if(!onBg){
        try{
            fg = &jobs.back();
            update();
        }catch(exception& e){

        }
    }
}



void JobsList::printJobsList() {
    update();

    if (jobs.empty())
        return;

    for (const JobEntry& i : jobs){
        cout << "[" << i.getJobId() << "] " << i.print();
        cout << " : " << i.getPid() << " ";
        cout << i.getTime() <<" secs";
        if(i.getStatus() == STOP || i.getStatus2() == STOP)
            cout << " (stopped)";
        cout << endl;
    }
}

void JobsList::killAllJobs() {
    for(JobEntry& i : jobs) {
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
        if(kill(job->getPid(),sig) == KILL_ERR)
            throw killError();
        if(kill(job->getPid2(),sig) == KILL_ERR)
            throw killError();
    }

    update(false);
}

void JobsList::bringFG(int jobId) {
    update(false);
    assert(fg == nullptr);

    if(jobs.empty())
        throw emptyList();

    if(jobId == 0)
        fg = &jobs.back();
    else
        fg = &getJobById(jobId);

    fg->continueCmd();
    cout << fg->print() << " : " << fg->getPid() << endl;

    update();
}

void JobsList::resumeOnBG(int jobId) {
    update();
    JobEntry* job;
    if(jobs.empty())
        throw emptyList();


    if(jobId == 0){
        job = &getLastStoppedJob();
        job->continueCmd();
    }else{
        job = &getJobById(jobId);
        if (job->getStatus() != STOP && job->getStatus2() != STOP)
            throw inBG();

        job->continueCmd();
    }
    cout << job->print() << " : " << job->getPid() << endl;
}

void JobsList::update(bool runInFg) {
    if(runInFg)
        runFG();


    removeFinishedJobs();

    if(fg && fg->getStatus() != RUN)
        fg = nullptr;

    if(jobs.empty())
        maxId = 0;
    else
        maxId = jobs.back().getJobId();
}

void JobsList::runFG() {
    if (!fg)
        return;

    fg->updateStatus(true);
    assert(fg->getStatus() != RUN);
    fg = nullptr;
}

void JobsList::removeFinishedJobs() {
    if(jobs.empty())
        return;

    vector<JobEntry> temp;
    for(JobEntry& i : jobs){
        i.updateStatus();
        if(i.getStatus() != END || i.getStatus2() != END)
            temp.push_back(i);
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
    if(jobs.empty())
        throw emptyList();

    if(jobId < 0)
        throw notExist();

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
        if (i.getStatus() == STOP || i.getStatus2() == STOP)
            lastStopped = &i;
    }

    if(!lastStopped){
        throw emptyList();
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

void JobsList::checkTimeOut() {
    time_t current = time(nullptr);
    for(JobEntry& i : jobs) {
        if (i.getTime() > 0 && difftime(current, i.getTime()) >= 0) { // current-timeout
            cout << "smash: " << i.print() << "!" << endl;
            i.killCmd();
            update(false);
            return;
        }
    }
}

pid_t JobsList::getPidById(int id) {
    return getJobById(id).getPid();
}
