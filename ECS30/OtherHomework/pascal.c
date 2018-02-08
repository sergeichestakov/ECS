/*
Sergei Chestakov
ECS 30
This program asks the user how many levels of pascal's triangle they would like to see and prints out that many rows
*/
#include <stdio.h>

//Prints Pascal's triangle
void printPascal(int levels){
	int row;
	//For each row the user wants to see
	for (row = 0; row < levels; ++row){
		int cell;
		double currElement, lastElement;
		for (cell = 0; cell <= row; ++cell){
			if (cell == 0){
				//First cell is always 1
				currElement = 1;
			} else {
				//Formula of each cell in a row using the row number and the cell number
				double mult = ((double)row + 1 - (double)cell) / ((double)cell);
				currElement = lastElement * mult;
			}
			printf("%.0lf ", currElement);
			//Keep track of the previous element
			lastElement = currElement;
		}
		//Print new line after each row
		printf("\n");
	}
}

int main(){
	int levels = 0;
	printf("Please enter how many levels of Pascal's Triangle you would like to see: ");
	scanf("%d", &levels);
	printPascal(levels);
	return 0;
}
