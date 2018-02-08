#ifndef READ_LINES_H
	#define READ_LINES_H
	#include <stdio.h>
	#include <stdlib.h>
	void read_lines(FILE* fp, char*** lines, int* num_lines);
	char* getLine(FILE* fp);
#endif
