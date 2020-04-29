#include "Signals.h"

using namespace std;

void ctrlZHandler(int sig_num) {
    cout << "smash: got ctrl-Z" << endl;

    if(JobsList::getInstance().fgPid() == -1)
        return;

    string pidStr = std::to_string(JobsList::getInstance().fgPid());
    JobsList::getInstance().sendSigById(SIGSTOP);
    cout <<  "smash: process " << pidStr << " was stopped"<< endl;
}

void ctrlCHandler(int sig_num) {
    cout<< "smash: got ctrl-C"<< endl;

    if(JobsList::getInstance().fgPid() == -1)
        return;

    string pidStr = std::to_string(JobsList::getInstance().fgPid());
    JobsList::getInstance().sendSigById(SIGKILL);
    cout<< "smash: process " << pidStr << " was killed"<< endl;
}

void alarmHandler(int sig_num) {
    cout << "smash: got an alarm" << endl;
    JobsList::getInstance().checkTimeOut();
}