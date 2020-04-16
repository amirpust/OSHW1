#include "Libs.h"
#include "SmallShell.h"
#include "Signals.h"

int main(int argc, char* argv[]) {
    PRINT_START;
    if(signal(SIGTSTP , ctrlZHandler)==SIG_ERR) {
        perror("smash error: failed to set ctrl-Z handler");
    }
    if(signal(SIGINT , ctrlCHandler)==SIG_ERR) {
        perror("smash error: failed to set ctrl-C handler");
    }
    /* if(signal(SIGINT , alarmHandler)==SIG_ERR) {
         perror("smash error: failed to set alarm handler");
     }*/

    //TODO: setup sig alarm handler

    SmallShell& smash = SmallShell::getInstance();


    while(true) {
        std::cout << smash.getName() << "> ";
        std::string cmd_line;
        std::getline(std::cin, cmd_line);
        try{
            smash.executeCommand(cmd_line.c_str());
        }catch(exception& /*commandException&*/ e) {
            std::cout << "smash" << e.what() << endl;
        }
    }
    PRINT_END;
    return 0;
}