fill: fill.o Shape.o Domain.o
	g++ -o fill fill.o Shape.o Domain.o
fill.o: fill.cpp Shape.h Shape.cpp
	g++ -Wall -o fill.o -c fill.cpp
Shape.o: Shape.cpp Shape.h
	g++ -Wall -o Shape.o -c Shape.cpp
Domain.o: Domain.cpp Domain.h
	g++ -Wall -o Domain.o -c Domain.cpp
clean:
	rm -f *.o fill
