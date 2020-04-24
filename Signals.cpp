#include "Signals.h"

using namespace std;

void ctrlZHandler(int sig_num) {
    cout << "smash: got ctrl-Z" << endl;

    if(getpid() == SmallShell::getInstance().getMyPid())
        return;

    JobsList::getInstance().sendSigById(SIGSTOP);
    cout <<  "smash: process " << JobsList::getInstance().fgPid() <<
    " was stopped"<< endl;
}

void ctrlCHandler(int sig_num) {
    cout<< "smash: got ctrl-C"<< endl;

    if(getpid() == SmallShell::getInstance().getMyPid())
        return;

    string pidStr = std::to_string(JobsList::getInstance().fgPid());
    JobsList::getInstance().sendSigById(sig_num);
    cout<< "smash: process " << pidStr << " was killed"<< endl;
}

void alarmHandler(int sig_num) {
//    cout << "smash got an alarm" <<  endl;
//    const char* cmd = SmallShell::getInstance();//TODO: get last command in the shell
//    cout << "smash: " + cmd + "timed out!"<< endl;
}