all: testShape

CXXFLAGS=-g -Wall

Shape.o: Shape.cpp Shape.h
testShape.o: testShape.cpp Shape.h

testShape: testShape.o Shape.o
	$(CXX) -o $@ $^  $(LDFLAGS)

clean:
	rm -f *.o testShape

