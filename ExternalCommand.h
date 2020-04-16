#ifndef OSHW1_EXTERNALCOMMAND_H
#define OSHW1_EXTERNALCOMMAND_H

#include "Libs.h"
#include "Command.h"

class ExternalCommand : public Command {
public:
    ExternalCommand(string& decrypted,string &originalCommandLine, string *args, int size);
   // copy C'tor still having
    ExternalCommand(const ExternalCommand& toCopy):
            Command(toCopy.decryptedCmd,toCopy.originalCommandLine,toCopy.splitLine
                    ,toCopy.size,external){};

    void execute() override;
    virtual  ~ExternalCommand() = default;
};


#endif //OSHW1_EXTERNALCOMMAND_H
