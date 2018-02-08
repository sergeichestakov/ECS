boarding.out : boarding.o
	g++ -ansi -Wall -g -o boarding.out boarding.o

boarding.o : boarding.cpp
	g++ -ansi -Wall -g -c boarding.cpp

a.out : timetest.o
	g++ -ansi -Wall -g -o a.out timetest.o

timetest.o : timetest.cpp dsexceptions.h CPUTimer.h
	g++ -ansi -Wall -g -c timetest.cpp

clean : 
	rm -f boarding.out boarding.o a.out timetest.o
