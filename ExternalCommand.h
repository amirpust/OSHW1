#ifndef OSHW1_EXTERNALCOMMAND_H
#define OSHW1_EXTERNALCOMMAND_H

#include "Libs.h"
#include "Command.h"

class ExternalCommand : public Command {
public:
    ExternalCommand(string& decrypted,string &originalCommandLine, string *args, int size);
   // copy C'tor
    ExternalCommand(ExternalCommand& toCopy):
    ExternalCommand(toCopy.decryptedCmd,toCopy.originalCommandLine,toCopy.splitLine
            ,toCopy.size){};
    void execute() override;
    virtual  ~ExternalCommand() = default;
};


#endif //OSHW1_EXTERNALCOMMAND_H
