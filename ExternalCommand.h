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

class CopyCommand : public Command{
    string oldFile;
    string newFile;
    int readFD = -1;
    int writeFD = -1;

public:

    CopyCommand(string& decrypted,string &originalCommandLine, string *args, int size)
            : Command(decrypted, originalCommandLine, args, size, copyCmd){
        //TODO: check arguments
        oldFile = splitLine[1];
        newFile = splitLine[2];
    }

    CopyCommand(CopyCommand const& toCopy)
            :Command(toCopy.decryptedCmd, toCopy.originalCommandLine, toCopy.splitLine, toCopy.size, copyCmd){
        oldFile = toCopy.oldFile;
        newFile = toCopy.newFile;
    };

    void execute()override {
        prepare();
        char buf[1];
        while(read(readFD,buf,1) > 0 && write(writeFD,buf,1) > 0){}
    }

private:
    void prepare(){
        //opening reading file
        readFD = open(oldFile.c_str(),O_RDONLY);
        if(readFD == -1){
            //TODO: should return some kind of error
        }
        writeFD = open(newFile.c_str(),O_CREAT | O_RDWR | O_TRUNC, S_IRWXU);
        if (writeFD == -1){
            //TODO: should return some kind of error
        }
    }

};
#endif //OSHW1_EXTERNALCOMMAND_H
