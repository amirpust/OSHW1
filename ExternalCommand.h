#ifndef OSHW1_EXTERNALCOMMAND_H
#define OSHW1_EXTERNALCOMMAND_H

#include "Libs.h"
#include "Command.h"

class ExternalCommand : public Command {


public:

    ExternalCommand(string& decrypted,string &originalCommandLine, string *args, int size);
    void execute() override;
};


#endif //OSHW1_EXTERNALCOMMAND_H
