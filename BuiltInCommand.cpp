#include "BuiltInCommand.h"
#include "SmallShell.h"


ChpromptCommand::ChpromptCommand(string *args, int size) :
BuiltInCommand(args,size){}

void ChpromptCommand::execute() {
    if(size > 1){
        SmallShell::getInstance().setName(splitLine[1]);
    }else
        SmallShell::getInstance().setName("");
}

ShowpidCommand::ShowpidCommand(string *args, int size) :
        BuiltInCommand(args,size){}

void ShowpidCommand::execute() {
    cout << "smash pid is "<< SmallShell::getInstance().getMyPid() << endl;
}

PwdCommand::PwdCommand(string *args, int size) :
        BuiltInCommand(args,size){}

void PwdCommand::execute() {
    char* dir =get_current_dir_name();
    if(dir == GET_CURR_DIR_ERR){
        throw getCurrentDirError();
    }
    cout << dir << endl;
    free(dir);
}

CdCommand::CdCommand(string *args, int size) :
BuiltInCommand(args,size){}

void CdCommand::execute() {
    if(size > 2){
        throw tooManyArgs(splitLine[0]);
    }if(splitLine[1] == "-"){
        if(SmallShell::getInstance().getPreviousDir().empty()){
            throw emptyDirHistory(splitLine[0]);
        }
    }
    SmallShell::getInstance().cd(splitLine[1]);
}

JobsCommand::JobsCommand(string *args, int size) :
        BuiltInCommand(args,size){}

void JobsCommand::execute() {
    try{
        JobsList::getInstance().printJobsList();
    }catch (JobsList::emptyList& e){
        throw emptyList(splitLine[0]);
    }
}

KillCommand::KillCommand(string *args, int size) :
        BuiltInCommand(args,size){
    if(size != 3)
        throw invalidArgs(splitLine[0]);
    try {
        size_t indexSig;
        size_t indexJobId;
        sig =  stoi(splitLine[1], &indexSig);
        jobId = stoi(splitLine[2], &indexJobId);
        if(indexJobId < splitLine[2].size() || indexSig < splitLine[1].size())
            throw invalidArgs(splitLine[0]);
    }catch(exception& e){
        throw invalidArgs(splitLine[0]);
    }

    sig *= -1;
    if(sig < 0){
        throw invalidArgs(splitLine[0]);
    }

}

void KillCommand::execute() {
    try {
        pid_t currentPid = JobsList::getInstance().getPidById(jobId);
        JobsList::getInstance().sendSigById(sig,jobId);
        cout << "signal number " << sig << " was sent to pid " <<
             currentPid << endl;
    }catch(JobsList::notExist& e){
        throw jobDoesntExist(splitLine[0],jobId);
    }catch(JobsList::emptyList& e){
        throw jobDoesntExist(splitLine[0],jobId);
    }


}

FgCommand::FgCommand(string *args, int size) :
        BuiltInCommand(args,size){
    if(size > 2){
        throw invalidArgs(splitLine[0]);
    }else if(size == 1){
        jobId = 0;
        return;
    }else{
        try {
            size_t index = 0;
            jobId = stoi(splitLine[1], &index);
            if(index < splitLine[1].size())
                throw invalidArgs(splitLine[0]);
        }catch(exception& e){
            throw invalidArgs(splitLine[0]);
        }

    }

}

void FgCommand::execute() {
    try {
        JobsList::getInstance().bringFG(jobId);
    }catch(JobsList::notExist& e){
        throw jobDoesntExist(splitLine[0], jobId);
    }catch(JobsList::emptyList& e){
        throw emptyList(splitLine[0]);
    }
}

BgCommand::BgCommand(string *args, int size) :
        BuiltInCommand(args,size){
    if(size > 2)
        throw invalidArgs(splitLine[0]);
    if(size == 1)
        jobId = 0;
    else{
        try {

            size_t index = 0;
            jobId = stoi(splitLine[1], &index);
            if(index < splitLine[1].size()) {
                throw invalidArgs(splitLine[0]);
            }
        }catch(exception& e){
            throw invalidArgs(splitLine[0]);
        }
    }
}

void BgCommand::execute() {
    try {
        JobsList::getInstance().resumeOnBG(jobId);
    }catch (JobsList::notExist& e){
        throw jobDoesntExist(splitLine[0],jobId);
    }catch (JobsList::inBG& e){
        throw jobAlreadyBGRuning(splitLine[0],jobId);
    }catch(JobsList::emptyList& e){
        throw emptyListBG(splitLine[0]);
    }
}

QuitCommand::QuitCommand(string *args, int size) :
BuiltInCommand(args,size){}

void QuitCommand::execute() {
    if(size > 1 && splitLine[1] == "kill"){
        cout << "smash: sending SIGKILL signal to " <<
             std::to_string(JobsList::getInstance().getSize()) << " jobs:" << endl;
        JobsList::getInstance().killAllJobs();
    }

    SmallShell::getInstance().setRunning(false);
}
