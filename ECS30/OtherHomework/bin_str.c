/*
Sergei Chestakov
ECS 30
This program takes in a binary string that may contain x's and if it does, then it outputs all possible versions of the string by replacing each x with a 1 or a 0
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printRec(char* binString){
	char* isX = strchr(binString, 'x');
	//Base case: If there is no x character, print the string
	if(isX == NULL){
		printf("%s\n", binString);
		return;
	} else {
		int index = isX - binString;
		//Replace x with a 0 in original string
		binString[index] = '0';
		//Create new String which is the same as the original but with a 1 instead
		int length = strlen(binString);
		char* newString = (char*)malloc(length * sizeof(char));
		strcpy(newString, binString);
		newString[index] = '1';
		//Recursively call function on both strings
		printRec(binString);
		printRec(newString);
	}	
}

int main(int argc, char* argv[]){
	printRec(argv[1]);
	return 0;
}
