#include <iostream>
#include <sstream>
#include <errno.h>
const std::string WHITESPACE = " \n\r\t\f\v";
using namespace std;
bool isBackground(string& cmd){
    return cmd[cmd.find_last_not_of(WHITESPACE)] == '&';
}

void removeBackgroundSign(string& cmd){
    int index = cmd.find_last_not_of(WHITESPACE);
    if(cmd[index] != '&')
        return;

    cmd[index] = '\0';

}

int parseCommand(const string& cmd,string* splitCmd){

    int i = 0;
    istringstream iss(cmd);
    string str;
    while(getline(iss,str,' ')){
        splitCmd[++i] = string(str);
    }
    return i;
}

extern int errno;

int main() {

    return 0;
}