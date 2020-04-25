//
// Created by Amir on 16/04/2020.
//

#include "Command.h"

Command::Command(const string *args, const int size, cmdType type)
        : size(size), type(type) {
    for (int i = 0; i < size; ++i) {
        splitLine[i] = string(args[i]);
    }
}

cmdType Command::getType() const {
    return type;
}
