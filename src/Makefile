CC              = gcc
CXX             = g++
CCFlags         = -g -O1 -w -Wall -pedantic -fPIC `root-config --cflags --evelibs`
#DevFlags                    = -D__CBCDAQ_DEV__
DevFlags                   =

IncludeDirs     =  /opt/cactus/include ../

IncludePaths            = $(IncludeDirs:%=-I%)

LibraryDirs = /opt/cactus/lib /opt/xdaq/lib ../lib
LibraryPaths = $(LibraryDirs:%=-L%)

ExternalObjects = $(LibraryPaths) -lpthread -lboost_thread -lboost_filesystem -lboost_regex -lboost_system -lboost_thread -lcactus_extern_pugixml -lcactus_uhal_log -lcactus_uhal_grammars -lcactus_uhal_uhal `root-config --glibs` -lPh2_Interface -lPh2_Description -lPh2_System -lPh2_Tools

%.o: %.cc %.h
	$(CXX) -std=c++11 $(DevFlags) $(CCFlags) $(UserCCFlags) $(CCDefines) $(IncludePaths) -c -o $@ $<


all: testpgrm datatest2cbc datatest8cbc mcp system calibrationtest interfacetest

testpgrm: test.cc
	$(CXX) -std=c++11 $(CCFlags) -o $@ $< $(IncludePaths) $(ExternalObjects)
	mv $@ ../bin

datatest2cbc: readdatatest2CBC.cc
	$(CXX) -std=c++11 $(CCFlags) -o $@ $< $(IncludePaths) $(ExternalObjects)
	mv $@ ../bin

datatest8cbc: readdatatest8CBC.cc
	$(CXX) -std=c++11 $(CCFlags) -o $@ $< $(IncludePaths) $(ExternalObjects)
	mv $@ ../bin

system: systemtest.cc
	$(CXX) -std=c++11 $(CCFlags) -o $@ $< $(IncludePaths) $(ExternalObjects)
	mv $@ ../bin

mcp: mcp.cc
	$(CXX) -std=c++11 $(CCFlags) -o $@ $< $(IncludePaths) $(ExternalObjects)
	mv $@ ../bin

calibrationtest: calibrationtest.cc
	$(CXX) -std=c++11 $(CCFlags) -o $@ $< $(IncludePaths) $(ExternalObjects)
	mv $@ ../bin

interfacetest: interfacetest.cc
	$(CXX) -std=c++11 $(CCFlags) -o $@ $< $(IncludePaths) $(ExternalObjects)
	mv $@ ../bin


clean:
