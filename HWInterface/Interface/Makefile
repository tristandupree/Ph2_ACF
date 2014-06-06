Objs                    = HWInterface.o
CC              = gcc
CXX             = g++
CCFlags         = -g -O1 -Wall -fPIC `root-config --cflags`

IncludeDirs     =  /opt/cactus/include /opt/xdaq/include ../

IncludePaths            = $(IncludeDirs:%=-I%)

LibraryDirs = /opt/cactus/lib /opt/xdaq/lib
LibraryPaths = $(LibraryDirs:%=-L%)

ExternalObjects = $(LibraryPaths) -lpthread -lboost_thread -lboost_filesystem -lboost_regex -lboost_system
-lboost_$

%.o: %.cpp %.h
	$(CXX) $(DevFlags) $(CCFlags) $(UserCCFlags) $(CCDefines) $(IncludePaths) -c -o $@ $<


all: $(Objs)
	$(CC) -shared -o libInterface.so $(Objs)
	mv libInterface.so ../lib/

clean:
	rm -f *.o
