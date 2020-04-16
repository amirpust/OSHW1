#include "BuiltInCommand.h"
#include "SmallShell.h"

ChpromptCommand::ChpromptCommand(string &decrypted, string &originalCommandLine, string *args, int size) :
BuiltInCommand(decrypted,originalCommandLine,args,size){}

void ChpromptCommand::execute() {
    if(size > 1){
        SmallShell::getInstance().setName(splitLine[1]);
    }else
        SmallShell::getInstance().setName("");
}

ShowpidCommand::ShowpidCommand(string &decrypted, string &originalCommandLine, string *args, int size) :
        BuiltInCommand(decrypted,originalCommandLine,args,size){}

void ShowpidCommand::execute() {
    cout << getpid() << endl;
}

PwdCommand::PwdCommand(string &decrypted, string &originalCommandLine, string *args, int size) :
        BuiltInCommand(decrypted,originalCommandLine,args,size){}

void PwdCommand::execute() {
    cout << get_current_dir_name() << endl;
}

CdCommand::CdCommand(string &decrypted, string &originalCommandLine, string *args, int size) :
BuiltInCommand(decrypted,originalCommandLine,args,size){}

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

JobsCommand::JobsCommand(string &decrypted, string &originalCommandLine, string *args, int size) :
        BuiltInCommand(decrypted,originalCommandLine,args,size){}

void JobsCommand::execute() {
    try{
        JobsList::getInstance().printJobsList(); //TODO: add the try catch policy
    }catch (JobsList::emptyList& e){
        throw emptyList(splitLine[0].c_str());
    }
}

KillCommand::KillCommand(string &decrypted, string &originalCommandLine, string *args, int size) :
        BuiltInCommand(decrypted,originalCommandLine,args,size){
    sig = stoi(splitLine[1],nullptr);
    sig *= -1;
    jobId = stoi(splitLine[1], nullptr);
}

void KillCommand::execute() {
    try {
        JobsList::getInstance().sendSigById(sig,jobId);
    }catch(JobsList::notExist& e){
        throw jobDoesntExist(splitLine[0].c_str(),jobId);
    }
}

FgCommand::FgCommand(string &decrypted, string &originalCommandLine, string *args, int size) :
        BuiltInCommand(decrypted,originalCommandLine,args,size){
    if(size != 2){
        throw invalidArgs(splitLine[0].c_str());
    }
    jobId = stoi(args[1], nullptr);
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

BgCommand::BgCommand(string &decrypted, string &originalCommandLine, string *args, int size) :
        BuiltInCommand(decrypted,originalCommandLine,args,size){
    if(size == 1)
        jobId = 0;
    else
        jobId = stoi(args[1], nullptr);
}

void BgCommand::execute() {
    try {
        JobsList::getInstance().resumeOnBG(jobId);
    }catch (JobsList::notExist& e){
        throw jobDoesntExist(splitLine[0].c_str(),jobId);
        //TODO : check what exception is being thrown empty or not exist
    }catch (JobsList::inBG& e){
        throw jobAlreadyBGRuning(splitLine[0].c_str(),jobId);
    }catch(JobsList::emptyList& e){
        throw emptyListBG(splitLine[0].c_str());
    }
}

QuitCommand::QuitCommand(string &decrypted, string &originalCommandLine, string *args, int size) :
BuiltInCommand(decrypted,originalCommandLine,args,size){}

void QuitCommand::execute() {
    if(size > 1 && splitLine[1] == "kill"){
        cout << "smash: sending SIGKILL signal to " <<
             std::to_string(JobsList::getInstance().getSize()) << " jobs:" << endl;
        JobsList::getInstance().killAllJobs();
    }
    kill(SmallShell::getInstance().getMyPid(), SIGKILL);
}
