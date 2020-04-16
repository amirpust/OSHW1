#ifndef OSHW1_SMALLSHELL_H
#define OSHW1_SMALLSHELL_H

#include "Libs.h"
#include "JobsList.h"
#include "BuiltInCommand.h"

const std::string WHITESPACE = " \n\r\t\f\v";

class SmallShell {

    //maybe will return something else
    void cmdDecryptor(const string& original,string* cmd,string** splitCmd,int* size,
        bool* bg){
        *cmd = string(original);        // will copy the original cmd to make changes in it
        *bg = isBackground(original);   // checks if there is '&'
        removeBackgroundSign(*cmd);     // removes '&'


    }

    int parseCommand(const string& cmd,string** splitCmd){

        int i = 0;


    }

    // i check the last one because of pipe of |&
    bool isBackground(const string& cmd){
        return cmd[cmd.find_last_not_of(WHITESPACE)] == '&';
    }

    void removeBackgroundSign(string& cmd){
        int index = cmd.find_last_not_of(WHITESPACE);
        if(cmd[index] != '&')
            return;

        cmd[index] = '\0';
    }

};


#endif //OSHW1_SMALLSHELL_H
