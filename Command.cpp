//
// Created by Amir on 16/04/2020.
//

#include "Command.h"

Command::Command(string& cmdLine,string &originalCommandLine, string *args, int size, cmdType type)
        :decryptedCmd(string(cmdLine)) ,originalCommandLine(string(originalCommandLine))
        , size(size), type(type) {
    for (int i = 0; i < size; ++i) {
        splitLine[i] = string(args[i]);
    }
}

