SUBMITTERS := 316397843_203304480
CC = g++
OBJS = BuiltInCommand.o Command.o ExternalCommand.o JobEntry.o JobsList.o \
Signals.o SmallShell.o Smash.o
SMASH_BIN = smash
COMP_FLAGS = --std=c++11 -Wall
TESTS_INPUTS := $(wildcard test_input*.txt)
TESTS_OUTPUTS := $(subst input,output,$(TESTS_INPUTS))

$(SMASH_BIN) : $(OBJS)
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
	./$(SMASH_BIN)

clean:
	rm -rf $(OBJS) $(SMASH_BIN)
	rm -rf $(SUBMITTERS).zip
	
test: $(TESTS_OUTPUTS)

$(TESTS_OUTPUTS): $(SMASH_BIN)
$(TESTS_OUTPUTS): test_output%.txt: test_input%.txt test_expected_output%.txt
	./$(SMASH_BIN) < $(word 1, $^) > $@
	diff $@ $(word 2, $^)
	echo $(word 1, $^) ++PASSED++

$(SMASH_BIN): $(OBJS)
	$(COMPILER) $(COMPILER_FLAGS) $^ -o $@

$(OBJS): %.o: %.cpp
	$(COMPILER) $(COMPILER_FLAGS) -c $^
	
zip: $(OBJS)
	zip $(SUBMITTERS).zip $^ submitters.txt Makefile