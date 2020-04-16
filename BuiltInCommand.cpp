//
// Created by Amir on 16/04/2020.
//

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
