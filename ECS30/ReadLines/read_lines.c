/*
Sergei Chestakov
ECS 30
This function reads a line from a file and stores it in a two dimensional array
It also updats a variable that keeps track of the number of lines read
*/
#include "read_lines.h"

void read_lines(FILE* fp, char*** lines, int* num_lines){
	//If file does not exist set array to null, number of lines to zero, and exit
	if(fp == NULL){
		lines = NULL;
		num_lines = 0;
		exit(0);
	}
	int linesRead = 0;
	//Create a pointer for lines to make it easier to read and understand
	char** linesPtr = *lines;
	//While we are not at the end of the file reallocate the array and continue reading from the file
	while(!feof(fp)){
		linesPtr = (char**)realloc(linesPtr, (linesRead + 1) * sizeof(char*));
		linesPtr[linesRead] = getLine(fp);
		linesRead++;
	}
	//Update the array and number of lines read
	*lines = linesPtr;
	*num_lines = linesRead - 1;
}

//This function reads a line from a file and returns it as a char pointer
char* getLine(FILE* fp){
	char* line = (char*) malloc(sizeof(char));
	int size = 1;
	int num_chars = 0; 
	do{
		//Scan from file 
    	fscanf(fp, "%c", &line[num_chars]);
		num_chars++;
		if(num_chars == size){
			size *= 2;
			line = (char*)realloc(line, size * sizeof(char));
    	}
		//Prevents infinite loops if new line char not detected 
		if(feof(fp)){
			break;
		}
	}while(line[num_chars - 1] != '\n');
	line[num_chars] = '\0';
	//shrink to fit only the number of characters entered
	line = (char*)realloc(line, (num_chars + 1) * sizeof(char));
	return line;
}
