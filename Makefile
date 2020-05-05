SUBMITTERS := 316397843_203304480
COMPILER = g++
OBJS=$(subst .cpp,.o,$(SRCS))
SMASH_BIN = smash
COMPILER_FLAGS = --std=c++11
TESTS_INPUTS := $(wildcard test_input*.txt)
TESTS_OUTPUTS := $(subst input,output,$(TESTS_INPUTS))
SRCS = BuiltInCommand.cpp Command.cpp ExternalCommand.cpp JobEntry.cpp JobsList.cpp \
Signals.cpp SmallShell.cpp Smash.cpp
HDRS = BuiltInCommand.h Libs.h SysCallException.h Command.h CommandExceptions.h \
JobsList.h JobEntry.h SmallShell.h ExternalCommand.h Signals.h


$(SMASH_BIN): $(OBJS)
	$(COMPILER) $(COMPILER_FLAGS) $^ -o $@

$(OBJS): %.o: %.cpp
	$(COMPILER) $(COMPILER_FLAGS) -c $^
clean:
	rm -rf $(OBJS) $(SMASH_BIN)
	rm -rf $(SUBMITTERS).zip
	

zip: $(SRCS) $(HDRS)
	zip $(SUBMITTERS).zip $^ submitters.txt Makefile