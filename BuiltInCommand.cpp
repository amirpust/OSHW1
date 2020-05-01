#include "BuiltInCommand.h"
#include "SmallShell.h"

#define MAX_SIG 31
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
        throw tooManyArgs(splitLine[0].c_str());
    }if(splitLine[1] == "-"){
        if(SmallShell::getInstance().getPreviousDir().empty()){
            throw emptyDirHistory(splitLine[0].c_str());
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
        throw emptyList(splitLine[0].c_str());
    }
}

KillCommand::KillCommand(string *args, int size) :
        BuiltInCommand(args,size){
    if(size != 3)
        throw invalidArgs(splitLine[0].c_str());
    try {
        size_t indexSig;
        size_t indexJobId;
        sig =  stoi(splitLine[1], &indexSig);
        jobId = stoi(splitLine[2], &indexJobId);
        if(indexJobId < splitLine[2].size() || indexSig < splitLine[1].size())
            throw invalidArgs(splitLine[0].c_str());
    }catch(exception& e){
        throw invalidArgs(splitLine[0].c_str());
    }

    sig *= -1;
    if(jobId == 0)
        throw jobDoesntExist(splitLine[0].c_str(),jobId);
   /* if (sig < 0 || sig > MAX_SIG)
        throw invalidArgs(splitLine[0].c_str());*/
}

void KillCommand::execute() {

    try {
        cout << "signal number " << sig << " was sent to pid " <<
             JobsList::getInstance().getPidById(jobId) << endl;
        JobsList::getInstance().sendSigById(sig,jobId);
    }catch(JobsList::notExist& e){
        throw jobDoesntExist(splitLine[0].c_str(),jobId);
    }catch(JobsList::emptyList& e){
        throw jobDoesntExist(splitLine[0].c_str(),jobId);
    }


}

FgCommand::FgCommand(string *args, int size) :
        BuiltInCommand(args,size){
    if(size > 2){
        throw invalidArgs(splitLine[0].c_str());
    }else if(size == 1){
        jobId = 0;
        return;
    }else{
        try {
            size_t index = 0;
            jobId = stoi(splitLine[1], &index);
            if(index < splitLine[1].size())
                throw invalidArgs(splitLine[0].c_str());
        }catch(exception& e){
            throw invalidArgs(splitLine[0].c_str());
        }

    }

}

void FgCommand::execute() {
    try {
        JobsList::getInstance().bringFG(jobId);
    }catch(JobsList::notExist& e){
        throw jobDoesntExist(splitLine[0].c_str(), jobId);
    }catch(JobsList::emptyList& e){
        throw emptyList(splitLine[0].c_str());
    }
}

BgCommand::BgCommand(string *args, int size) :
        BuiltInCommand(args,size){
    if(size == 1)
        jobId = 0;
    else{
        try {

            size_t index = 0;
            jobId = stoi(splitLine[1], &index);
            if(index < splitLine[1].size()) {
                throw invalidArgs(splitLine[0].c_str());
            }
        }catch(exception& e){
            throw invalidArgs(splitLine[0].c_str());
        }
    }
}

void BgCommand::execute() {
    try {
        JobsList::getInstance().resumeOnBG(jobId);
    }catch (JobsList::notExist& e){
        throw jobDoesntExist(splitLine[0].c_str(),jobId);
    }catch (JobsList::inBG& e){
        throw jobAlreadyBGRuning(splitLine[0].c_str(),jobId);
    }catch(JobsList::emptyList& e){
        throw emptyListBG(splitLine[0].c_str());
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
