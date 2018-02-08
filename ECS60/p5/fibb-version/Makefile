runner.out: RoadRunner.o fibheap.o tripper.o
	g++ -Wall -g  -ansi  -o runner.out RoadRunner.o fibheap.o tripper.o 

RoadRunner.o: RoadRunner.cpp RoadRunner.h CPUTimer.h tripper.h
	g++ -Wall -g  -ansi  -c RoadRunner.cpp

tripper.o: tripper.cpp fibheap.cpp tripper.h RoadRunner.h fibheap.h
	g++ -Wall -g  -ansi  -c tripper.cpp

fibheap.o: fibheap.cpp fibheap.h
	g++ -Wall -g  -ansi  -c fibheap.cpp

clean:
	rm -f runner.out RoadRunner.o tripper.o 
