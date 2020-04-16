#include "Signals.h"

using namespace std;

void ctrlZHandler(int sig_num) {
    cout << ">>>>>>>>>Start CTRL+Z <<<<<<<<<<<<<" <<endl;
    cout << "smash: got ctrl-Z" << endl;
    //kill(SmallShell::getInstance().getJobs().fgPid(), SIGSTOP);
    JobsList::getInstance().sendSigById(SIGSTOP);
    cout <<  JobsList::getInstance().fgPid() << endl;
    cout << ">>>>>>>>>End CTRL+Z <<<<<<<<<<<<<" <<endl;
}

void ctrlCHandler(int sig_num) {
    cout<< "smash: got ctrl-C"<<endl;
    cout<< "smash: process " + std::to_string(JobsList::getInstance().fgPid()) + " was killed"<<endl;
    JobsList::getInstance().sendSigById(sig_num);
    //kill(SmallShell::getInstance().getJobs().fgPid() ,SIGKILL);
    //exit(9);
}

void alarmHandler(int sig_num) {
//    cout << "smash got an alarm" <<  endl;
//    const char* cmd = SmallShell::getInstance();//TODO: get last command in the shell
//    cout << "smash: " + cmd + "timed out!"<< endl;
}