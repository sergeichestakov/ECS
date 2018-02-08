/*
Sergei Chestakov
ECS 30
This program generates two pieces in random positions on a board with R rows and C columns (inputted by user) and moves each piece a random amount closer towards the other each round 
Outputs on average how many rounds it takes for the pieces to meet in a simulation
*/
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

bool is_valid_formatting(int num_args_read, int num_args_needed){
	/* check if the format on the entered input is correct
		@num_args_read: the number of format specifiers filled in by scanf
		@num_args_needed: the number of format specifiers you expected to be filled in
		@returns: true if the format is correct and false otherwise
		@side effects: consumes all characters on the standard input
		*/
	char new_line = '\n';
	bool is_valid = true; //assume the format is correct
	if(num_args_read != num_args_needed){ //didn't fill in all the format specifiers
		is_valid = false;//format is not correct
	}
	
	
	do{
		scanf("%c", &new_line); //read the next character from the standard input
		if(!isspace(new_line)){ //if it isn't white space
			is_valid = false; //extra stuff there so format isn't valid
		}
	}while(new_line != '\n'); //keep reading characters until you reach the new line character
	return is_valid;
}

int get_valid_int(char message[], bool seed){
	/*
		get an int from the user
		keep asking until they enter one
	*/
	int num;
	int num_args_read;
	do{
		printf("%s", message);
		num_args_read = scanf("%int", &num);
	}while(!(is_valid_formatting(num_args_read, 1)) || (!seed && num <= 0));
	
	return num;
}

//Finds a random integer between two pieces
int randInt(int P1, int P2, bool start){
	//If pieces are the same just return the piece
	if(P1 == P2){
		return P1;
	}
	int difference = abs(P1 - P2);
	int random;
	//Slightly different formula for generating starting position
	if (start){
		random = rand() % (difference);
	} else {
		random = rand() % (difference + 1);
	}
	//Return value based on which number inputted is higher
	if (P1 < P2){
		return P1 + random;
	} else {
		return P1 - random;
	}
}

//This is the method that actually runs the simulations and outputs the results
void run(int numRows, int numCols, int numSims){
	int simulation, P1row, P1col, P2row, P2col, totalMoves = 0;
	//For every simulation
	for(simulation = 0; simulation < numSims; ++simulation){
		//Reset the moves
		int moves = 0;
		//Keep track of the simulation with 5 or fewer sims
		if(numSims <= 5){
			printf("Simulation %d\n", simulation);	
		}
		P1row = randInt(0, numRows, true);
		P1col = randInt(0, numCols, true);
		P2row = randInt(0, numRows, true);
		P2col = randInt(0, numCols, true);
		//Print starting position if the number of simulations is 5 or fewer to help with debugging
		if(numSims <= 5){
			printf("Piece one starts at: %d, %d\n", P1row, P1col);
			printf("Piece two starts at: %d, %d\n", P2row, P2col);	
		}
		//While either they are not on the same row OR they are not  on the same column
		while(P1row != P2row || P1col != P2col){
			int newP1row = P1row;
			int newP1col = P1col;
			int newP2row = P2row;
			int newP2col = P2col;
			//Generate new positions for each
			newP1row = randInt(P1row, P2row, false);
			newP1col = randInt(P1col, P2col, false);
			newP2row = randInt(P2row, P1row, false);
			newP2col = randInt(P2col, P1col, false);
			//If number of simulations is 5 or fewer print their positions at each step to help with debugging
			if(numSims <= 5){
				printf("First piece moves from %d,%d to %d,%d\n", P1row, P1col, newP1row, newP1col);
				printf("Second piece moves from %d,%d to %d,%d\n", P2row, P2col, newP2row, newP2col);
			}
			//Update the positions
			P1row = newP1row;
			P1col = newP1col;
			P2row = newP2row;
			P2col = newP2col;
			//Count moves
			++moves;
		}
		//Keep track of all the moves from every simulation for the average
		totalMoves += moves;
	}
	//Calculate and output the average number of moves
	double average = (double) totalMoves / (double) numSims;
	printf("On average it takes %.2lf rounds on a board %d X %d for the pieces to meet.\n", average, numRows, numCols);
}

int main(){
	int seed = 0, numRows = 0, numCols = 0, numSims = 0;
	//Take in inputs and set values
	seed = get_valid_int("Enter the seed for the random number generator: ", true);
	numRows = get_valid_int("Enter the number of rows on the board: ", false);
	numCols = get_valid_int("Enter the number of columns on the board: ", false);
	numSims = get_valid_int("Enter the number of simulations to run: ", false);
	srand(seed);
	run(numRows, numCols, numSims);
	return 0;
}