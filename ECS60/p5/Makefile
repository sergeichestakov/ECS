runner.out: RoadRunner.o tripper.o VBinaryHeap.o
	g++ -Wall -g  -ansi  -o runner.out RoadRunner.o tripper.o VBinaryHeap.o

RoadRunner.o: RoadRunner.cpp RoadRunner.h CPUTimer.h tripper.h
	g++ -Wall -g  -ansi  -c RoadRunner.cpp

tripper.o: tripper.cpp tripper.h VBinaryHeap.h RoadRunner.h
	g++ -Wall -g  -ansi  -c tripper.cpp

VBinaryHeap.o: VBinaryHeap.cpp VBinaryHeap.h
	g++ -Wall -g  -ansi  -c VBinaryHeap.cpp

clean:
	rm -f runner.out RoadRunner.o tripper.o VBinaryHeap.o
