#include <iostream>

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

int main() {
    string s = "amir &\n";
    cout << isBackground(s) << endl;
    removeBackgroundSign(s);
    cout <<s <<endl;
    return 0;
}