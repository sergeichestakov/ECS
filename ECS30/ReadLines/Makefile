read_lines.out: read_lines.o main.o
	gcc -g -Wall -Werror -o read_lines.out read_lines.o main.o
	
main.o: main.c read_lines.h
	gcc -g -Wall -Werror -c -o main.o main.c
	
read_lines.o: read_lines.c read_lines.c
	gcc -g -Wall -Werror -c -o read_lines.o read_lines.c
	
clean:
	rm -f *.out *.o
