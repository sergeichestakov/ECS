#include <stdio.h>
#include <stdlib.h>
#include "read_lines.h"


void print_lines(char** lines, int num_lines){
	int i;
	for(i = 0 ; i < num_lines; ++i){
		printf("%d. %s", i+1, lines[i]);
	}
}

void free_lines(char** lines, int num_lines){
	int i;
	
	for(i = 0 ; i < num_lines; ++i){
		free(lines[i]);
	}
	
	if(lines != NULL && num_lines > 0){
		free(lines);
	}

}

FILE* validate_input(int argc, char* argv[]){

	FILE* fp = NULL;

	if(argc < 2){
		printf("Not enough arguments entered.\nEnding program.\n");
		exit(0);
	}
	else if(argc > 2){
		printf("Too many arguments entered.\nEnding program.\n");
		exit(0);
	}
	
	fp = fopen(argv[1], "r");
	
	if(fp == NULL){
		printf("Unable to open file: %s\nEnding program.\n", argv[1]);
		exit(0);
	}
	
	return fp;

}


int main(int argc, char* argv[]){
	char** lines = NULL;
	int num_lines = 0;
	FILE* fp = validate_input(argc, argv);
	
	read_lines(fp, &lines, &num_lines);
	print_lines(lines, num_lines);
	free_lines(lines, num_lines);
	fclose(fp);

	return 0;
}
