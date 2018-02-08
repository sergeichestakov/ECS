/*
Sergei Chestakov
ECS 30
This program adds two inputted matrices of any size together and ouputs the result
*/
#include <stdio.h>

//Collects inputs 
void inputMatrix(int rows, int cols, int matrix[rows][cols]){
	int r, c;
	for (r = 0; r < rows; ++r){
		for (c = 0; c < cols; ++c){
			scanf("%d", &matrix[r][c]);
		}
	}
}

//Prints matrices
void printMatrix(int rows, int cols, int matrix[rows][cols]){
	int r, c;
	for (r = 0; r < rows; ++r){
		for (c = 0; c < cols; ++c){
			printf("%d ", matrix[r][c]);
		}
		printf("\n");
	}
}

//Adds two inputted matrices together and outputs the result
void addMatrices(int rows, int cols, int matrixA[rows][cols], int matrixB[rows][cols]){
	int matrixC[rows][cols];	
	int r, c;
	for (r = 0; r < rows; ++r){
		for (c = 0; c < cols; ++c){
			matrixC[r][c] = matrixA[r][c] + matrixB[r][c];
		}
	}
	//Output
	printf("A + B =\n");
	printMatrix(rows, cols, matrixC);
}

int main(){
int rows, cols;
//Get input from the user about the number of rows and columns of each matrix
printf("Please enter the number of rows: ");
scanf("%d", &rows);
printf("Please enter the number of columns: ");
scanf("%d", &cols);
int matrixA[rows][cols];
int matrixB[rows][cols];
//Prompt user to input values into the matrices
printf("Enter Matrix A\n");
inputMatrix(rows, cols, matrixA);
printf("Enter Matrix B\n");
inputMatrix(rows, cols, matrixB);
//Add and output
addMatrices(rows, cols, matrixA, matrixB);
return 0;
}
