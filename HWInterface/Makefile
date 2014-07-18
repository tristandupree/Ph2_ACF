Objs                    = RegManager.o GLIBInterface.o CbcInterface.o Exception.o Utilities.o Event.o Data.o
CC              = gcc
CXX             = g++
CCFlags         = -g -O1 -w -Wall -pedantic -fPIC `root-config --cflags --evelibs` -Wcpp
DevFlags                   = -D__CBCDAQ_DEV__
#DevFlags		=

IncludeDirs     =  /opt/cactus/include /opt/xdaq/include ../

IncludePaths            = $(IncludeDirs:%=-I%)

%.o: %.cc %.h
	$(CXX) -std=c++11  $(DevFlags) $(CCFlags) $(UserCCFlags) $(CCDefines) $(IncludePaths) -c -o $@ $<

all: $(Objs) ../HWDescription/Definition.h
	$(CC) -std=c++11 -shared -o libPh2_Interface.so $(Objs) -pthread
	mv libPh2_Interface.so ../lib

clean:
	rm -f *.o
