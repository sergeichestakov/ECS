all: calculator useFraction

calculator: calculator.o Fraction.o
	g++ -o calculator Fraction.o calculator.o
useFraction: useFraction.o Fraction.o
	g++ -o useFraction useFraction.o Fraction.o
Fraction.o: Fraction.cpp Fraction.h
	g++ -Wall -o Fraction.o -c Fraction.cpp
useFraction.o: useFraction.cpp Fraction.h Fraction.o
	g++ -Wall -o useFraction.o -c useFraction.cpp
calculator.o: calculator.cpp
	g++ -Wall -o calculator.o -c calculator.cpp
clean: 
	rm -f *.o calculator useFraction

