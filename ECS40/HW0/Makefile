hello: hello.o
	g++ -g -Wall -Werror -o hello hello.o
hello.o: hello.cpp
	g++ -g -Wall -Werror -c -o hello.o hello.cpp
clean: 
	rm -f *.o *.out hello
