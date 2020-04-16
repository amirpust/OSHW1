//
// Created by Amir on 16/04/2020.
//

#include "ExternalCommand.h"

void ExternalCommand::execute() {
    char bash[5] = "bash";
    char flag[3] = "-c";
    char cmd[COMMAND_ARGS_MAX_LENGTH + 1];
    strcpy(cmd,decryptedCmd.c_str());
    char* cmd_args[4];
    cmd_args[0] = bash;
    cmd_args[1] = flag;
    cmd_args[2] = cmd;
    cmd_args[3] = NULL;
    execvp("/bin/bash",cmd_args);
}

ExternalCommand::ExternalCommand(string& decrypted,string &originalCommandLine, string *args, int size) :
Command(decrypted,originalCommandLine,args,size,external){}
