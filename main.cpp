#include <iostream>
#include <sstream>
#include <errno.h>
#include <cstring>

using namespace std;

int main() {
    char cmdArr[200][200];
    string a = "amir";
    string s[3] = {a, "was", "here"};
    for (int i = 0; i < 3; ++i) {
        strcpy(cmdArr[i], s[i].c_str());
    }

    for (int j = 0; j < 3; ++j) {
        cout << cmdArr[j] << endl;
    }
    return 0;
}