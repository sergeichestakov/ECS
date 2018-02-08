runner.out: RoadRunner.o tripper.o
	g++ -Wall  -ansi  -o runner.out RoadRunner.o tripper.o 

RoadRunner.o: RoadRunner.cpp RoadRunner.h CPUTimer.h tripper.h 
	g++ -Wall  -ansi  -c RoadRunner.cpp

tripper.o: tripper.cpp tripper.h RoadRunner.h 
	g++ -Wall  -ansi  -c tripper.cpp

clean:
	rm -f runner.out RoadRunner.o tripper.o 
