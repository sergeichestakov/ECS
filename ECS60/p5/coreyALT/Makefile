runner.out: RoadRunner.o tripper.o VBinaryHeap.o
	g++ -Wall -pg  -ansi  -o runner.out RoadRunner.o tripper.o VBinaryHeap.o

RoadRunner.o: RoadRunner.cpp RoadRunner.h CPUTimer.h tripper.h VBinaryHeap.h
	g++ -Wall -pg  -ansi  -c RoadRunner.cpp

tripper.o: tripper.cpp tripper.h VBinaryHeap.h RoadRunner.h
	g++ -Wall -pg  -ansi  -c tripper.cpp

VBinaryHeap.o: VBinaryHeap.cpp VBinaryHeap.h
	g++ -Wall -pg  -ansi  -c VBinaryHeap.cpp

clean:
	rm -f runner.out RoadRunner.o tripper.o  VBinaryHeap.o
