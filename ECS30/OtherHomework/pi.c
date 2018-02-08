/*
Sergei Chetakov
ECS 30
This program uses the Monte Carlo method of estimating the value of pi by finding random points in a square and seeing how many of them are in a circle
*/
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
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

//Finds a random double using formula
double randDouble(double low, double high){
	return low + ( (double) rand() ) / ( ( (double) RAND_MAX) / (high - low) );
}

//Determines whether the point is in the circle
bool inCircle(double x, double y){
	double distance = sqrt((x*x) + (y*y));
	//Compare distance from the radius to the radius which is 1
	if (distance <= 1){
		return true;
	} else {
		return false;
	}
}

//This function finds and outputs the estimate of pi
void findPi(int iterations){
	int pointsIn = 0, totalPoints = 0, i = 0;
	//For the inputted number of iterations
	for (i = 0; i < iterations; ++i){
		//Generate two random numbers, one for the x axis and one for the y axis
		double xAxis = randDouble(-1, 1);
		double yAxis = randDouble(-1, 1);
		//Count how many are in circle and count the total
		if(inCircle(xAxis, yAxis)){
			++pointsIn;
		}
		++totalPoints;
	}
	//Calculate pi and output
	double pi = ((double) pointsIn / (double) totalPoints) * 4;
	printf("The value of pi is %.5lf.\n", pi);
}


int main(){
	int seed = 0, iterations = 0;
	seed = get_valid_int("Enter the seed for the random number generator: ", true);
	iterations = get_valid_int("Enter the number of iterations to run: ", false);
	srand(seed);
	findPi(iterations);
	return 0;
}