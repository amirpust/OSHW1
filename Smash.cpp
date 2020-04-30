#include "Libs.h"
#include "SmallShell.h"
#include "Signals.h"
#include "SysCallException.h"

int main(int argc, char* argv[]) {

    if(signal(SIGTSTP , ctrlZHandler)==SIG_ERR) {
        perror("smash error: failed to set ctrl-Z handler");
    }
    if(signal(SIGINT , ctrlCHandler)==SIG_ERR) {
        perror("smash error: failed to set ctrl-C handler");
    }
    if(signal(SIGALRM , alarmHandler)==SIG_ERR) {
         perror("smash error: failed to set alarm handler");
     }

    //TODO: setup sig alarm handler

    SmallShell& smash = SmallShell::getInstance();


    while(smash.isRunning() && getpid() == smash.getMyPid()) {
        std::cout << smash.getName() << "> ";
        std::string cmd_line;
        std::getline(std::cin, cmd_line);
        try{
            smash.executeCommand(cmd_line.c_str());
        }catch(commandException& e) {
            std::cerr << "smash" << e.print() << endl; //from the forum it says to print it to cerr not cout
        }catch (SysCallException& e){
            string massage = "smash error: " + e.print() + " failed";
            perror(massage.c_str()); // perror adds additional information to the syscall errors like file not found
        }
        smash.cleanUpIO();
    }
    return 0;
}