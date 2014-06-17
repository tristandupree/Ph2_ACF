mytest: mytest.o FEDescription.o Cbc.o Module.o
	g++ -o mytest mytest.o FEDescription.o Cbc.o Module.o

mytest.o: mytest.cc Module.h FEDescription.h Cbc.h
	g++ -Wall -c mytest.cc

Module.o: Module.cc Module.h
	g++ -Wall -c Module.cc

Cbc.o: Cbc.cc Cbc.h
	g++ -Wall -c Cbc.cc

FEDescription.o: FEDescription.cc FEDescription.h
	g++ -Wall -c FEDescription.cc


clean:
	rm -f *.o
