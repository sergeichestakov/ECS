/*
Sergei Chestakov
ECS 30
This program prints out every prime number in between 2 and a user entered positive integer greater than 1
*/
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>

//Checks to make sure format is valid based on the number of arguments read
bool isValidFormatting(int numArgsRead){
	char new_line;
	bool is_valid = true; //assume the format is correct
	if(numArgsRead != 1){ //didn't fill in all the format specifiers
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

//Returns a valid inputted integer
int getValidInt(){
	int num;
	int numArgsRead;
	do{
		//Keep printing message until input is valid
		printf("%s", "Enter a number greater than 1: ");
		numArgsRead = scanf("%d", &num);
	//Number must be greater than 1
	}while(!(isValidFormatting(numArgsRead)) || num < 2);
	return num;
}

//Returns whether a number is prime or not
bool isPrime(int num){
	int i, cnt = 0;
	//Counts how many times the number is evenly divided by the numbers before it
	for (i = 1; i <= num; ++i){
		if(num % i == 0){
			cnt++;
		}
	}
	//A prime number is only divided by 1 and itself 
	return cnt == 2;
}

//Prints every prime number below the number
void printPrimes(int num){
	int i;
	//Starts at 2 and includes the number itself
	for (i = 2; i <= num; ++i){
		//Print if prime
		if (isPrime(i)){
			printf("%d\n", i);
		}
	}
}

int main(){
	int num = getValidInt();
	printf("The primes between 2 and %d are: \n", num);
	printPrimes(num);
	return 0;
}
