exe : mainServ.o funcsServ.o
	g++ -o exe mainServ.o funcsServ.o
mainServ.o: mainServ.cpp funcsServ.h
	g++ -Wall -c mainServ.cpp
funcsServ.o : funcsServ.cpp funcsServ.h
	g++ -Wall -c funcsServ.cpp
clean:
	rm exe mainServ.o funcsServ.o

