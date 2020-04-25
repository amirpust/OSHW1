CC = g++
OBJS = BuiltInCommand.o Command.o ExternalCommand.o JobEntry.o JobsList.o \
Signals.o SmallShell.o Smash.o
EXEC = smash
COMP_FLAGS = --std=c++11 -Wall
$(EXEC) : $(OBJS)
	$(CC) $(OBJS) -o $@
BuiltInCommand.o:BuiltInCommand.cpp BuiltInCommand.h Libs.h Debugger.h \
	SysCallException.h Command.h CommandExceptions.h JobsList.h JobEntry.h \
	ExternalCommand.h SmallShell.h
	$(CC) -c $(COMP_FLAGS)  $*.cpp
Command.o:Command.cpp Command.h Libs.h Debugger.h SysCallException.h \
	CommandExceptions.h
	$(CC) -c $(COMP_FLAGS) $*.cpp
ExternalCommand.o: ExternalCommand.cpp ExternalCommand.h Libs.h \
	Debugger.h SysCallException.h Command.h CommandExceptions.h
	$(CC) -c $(COMP_FLAGS) $*.cpp
JobEntry.o:JobEntry.cpp JobEntry.h Libs.h Debugger.h SysCallException.h \
	ExternalCommand.h Command.h CommandExceptions.h
	$(CC) -c $(COMP_FLAGS) $*.cpp
JobsList.o:JobsList.cpp JobsList.h Libs.h Debugger.h SysCallException.h \
	JobEntry.h ExternalCommand.h Command.h CommandExceptions.h
	$(CC) -c $(COMP_FLAGS) $*.cpp
Signals.o:Signals.cpp Signals.h Libs.h Debugger.h SysCallException.h \
	SmallShell.h JobsList.h JobEntry.h ExternalCommand.h Command.h \
	CommandExceptions.h
	$(CC) -c $(COMP_FLAGS) $*.cpp
SmallShell.o:SmallShell.cpp SmallShell.h Libs.h Debugger.h \
	SysCallException.h JobsList.h JobEntry.h ExternalCommand.h Command.h \
	CommandExceptions.h BuiltInCommand.h
	$(CC) -c $(COMP_FLAGS) $*.cpp
Smash.o:Smash.cpp Libs.h Debugger.h SysCallException.h SmallShell.h \
	JobsList.h JobEntry.h ExternalCommand.h Command.h CommandExceptions.h \
	Signals.h

run:
	./$(EXEC)

clean:
	rm -rf $(OBJS) $(EXEC)