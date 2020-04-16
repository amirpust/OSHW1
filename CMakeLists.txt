cmake_minimum_required(VERSION 3.14)
project(OSHW1)

set(CMAKE_CXX_STANDARD 11)

add_executable(OSHW1 main.cpp Command.cpp Command.h BuiltInCommand.cpp BuiltInCommand.h ExternalCommand.cpp ExternalCommand.h JobsList.cpp JobsList.h JobEntry.cpp JobEntry.h SmallShell.cpp SmallShell.h Signals.cpp Signals.h Debugger.h Smash.cpp Libs.h)