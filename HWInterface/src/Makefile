CC              = gcc
CXX             = g++
CCFlags         = -g -O1 -Wall -fPIC `root-config --cflags`
DevFlags                    = -D__CBCDAQ_DEV__
#DevFlags                   =

IncludeDirs     =  /opt/cactus/include /opt/xdaq/include ../

IncludePaths            = $(IncludeDirs:%=-I%)

LibraryDirs = /opt/cactus/lib /opt/xdaq/lib ../lib
LibraryPaths = $(LibraryDirs:%=-L%)

ExternalObjects = $(LibraryPaths) -lpthread -lboost_thread -lboost_filesystem -lboost_regex -lboost_system -lboost_thread -lcactus_extern_pugixml -lcactus_uhal_log -lcactus_uhal_grammars -lcactus_uhal_uhal -lcgicc `root-config --glibs` -lPh2_Interface

%.o: %.cc %.h
	$(CXX) $(DevFlags) $(CCFlags) $(UserCCFlags) $(CCDefines) $(IncludePaths) -c -o $@ $<


all: mytest

mytest: test.cc
	$(CXX) $(CCFlags) -o $@ $< $(IncludePaths) $(ExternalObjects)
	mv $@ ../bin

clean:
