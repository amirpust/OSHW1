#ifndef OSHW1_EXTERNALCOMMAND_H
#define OSHW1_EXTERNALCOMMAND_H

#include <sys/stat.h>
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

class CopyCommand : public Command{
    string oldFile;
    string newFile;
    int readFD = -1;
    int writeFD = -1;

public:

    CopyCommand(string& decrypted,string &originalCommandLine, string *args, int size);

    CopyCommand(CopyCommand const& toCopy);;

    void execute()override;

private:
    void prepare();
    void cleanUp(){
        if(readFD > -1)
            if(close(readFD) == CLOSE_ERR)
                throw closeError();
        if(writeFD > -1)
            if(close(writeFD) == CLOSE_ERR)
                throw closeError();
    }
};
#endif //OSHW1_EXTERNALCOMMAND_H
