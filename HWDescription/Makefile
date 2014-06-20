mytest: mytest.o FEDescription.o Cbc.o Module.o
	g++ -std=c++0x -o mytest mytest.o FEDescription.o Cbc.o Module.o

mytest.o: mytest.cc Module.h FEDescription.h Cbc.h
	g++ -std=c++0x -Wall -c mytest.cc

Module.o: Module.cc Module.h
	g++ -std=c++0x -Wall -c Module.cc

Cbc.o: Cbc.cc Cbc.h
	g++ -std=c++0x -Wall -c Cbc.cc

FEDescription.o: FEDescription.cc FEDescription.h
	g++ -std=c++0x -Wall -c FEDescription.cc


clean:
	rm -f *.o
