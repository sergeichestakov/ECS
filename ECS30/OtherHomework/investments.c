/*
Sergei Chestakov
ECS 30
This program assesses whether a user should pay off only the minimum payment of a loan and invest the rest into retirement right away or pay off as much of the loan as possible before investing for retirement
*/
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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

double get_valid_double(char message[]){
	/*
		get a double from the user
		keep asking until they enter one
	*/
	double num;
	int num_args_read;
	do{
		printf("%s", message);
		num_args_read = scanf("%lf", &num);
	}while(!(is_valid_formatting(num_args_read, 1)) || (num <= 0));
	
	return num;
}

int get_valid_int(char message[]){
	/*
		get an int from the user
		keep asking until they enter one
	*/
	int num;
	int num_args_read;
	do{
		printf("%s", message);
		num_args_read = scanf("%d", &num);
	}while(!(is_valid_formatting(num_args_read, 1)) || (num <= 0));
	
	return num;
}

//Check for valid retirement age
int checkRetAge(int currAge, int retAge, char message[]){
	while (retAge <= currAge){
		retAge = get_valid_int(message);
	}
	return retAge;
}

//Check to make sure enough money is set aside, end the program if not
void checkMoneyReserved(double moneyReserved, double minLoanPayment){
	if (moneyReserved < minLoanPayment){
		printf("You didn't set aside enough money to pay off our loans. Ending program.\n");
		exit(0);
	}
}

//Calculate the results 
double calculateSavings(double moneyReserved, double moneyOwed, double interestRate, double rateReturn, int currAge, int retAge, double amountToPay){
	double savings = 0;
	double monthlyInterest = interestRate / 12.0;
	double monthlyReturn = rateReturn / 12.0;
	double toSavings = 0;
	int monthsToRetirement = (retAge - currAge) * 12;
	int months;
	//Every month until retirement
	for (months = 0; months < monthsToRetirement; ++months){
		//While you still have to pay a loan
		if(moneyOwed != 0){
			//Apply interest first
			moneyOwed *= (1 + monthlyInterest);
			//Substract the amount you want to pay from loan, unless the amount of loan left is less than that, then you substract that and add the difference to savings
			if(moneyOwed > amountToPay){
				moneyOwed -= amountToPay;
			} else {
				toSavings += amountToPay - moneyOwed;
				moneyOwed = 0;
			}
		} else {
			amountToPay = 0;
		}
		toSavings += (moneyReserved - amountToPay);
		savings *= (1 + monthlyReturn);
		savings += toSavings;
		toSavings = 0;
	}
	//Warning printed if there is still a loan left when you retire
	if (moneyOwed != 0){
		printf("Warning! After you retire you will still have $%.2lf in loans left.\n", moneyOwed);
	}
	return savings;
}

//Output the correct advice depending on how much each option leaves you with when you retire
void output(double minPay, double maxPay, double moneyReserved){
	if(minPay > maxPay){
		printf("You should only make the minimum payments on your loan and apply the rest towards retirement.\n");
		printf("If you do you will have $%.2lf when you retire as opposed to $%.2lf if you payed off your loan before investing.\n", minPay, maxPay);
	} else {
		printf("You should apply all $%.2lf towards your loan before making any investments.\n", moneyReserved);
		printf("If you do you will have $%.2lf when you retire as opposed to $%.2lf if you only made minimum payments.\n", maxPay, minPay);
	}
}

int main(){
	double moneyReserved = 0, moneyOwed = 0, interestRate = 0, minLoanPayment = 0, rateReturn = 0;
	int currAge = 0, retAge = 0;
	//Take in user inputs to assign values
	moneyReserved = get_valid_double("Enter how much money you will be putting towards loans/retirement each month: ");
	moneyOwed = get_valid_double("Enter how much you owe in loans: ");
	interestRate = get_valid_double("Enter the annual interest rate of the loans: ");
	minLoanPayment = get_valid_double("Enter your minimum monthly loan payment: ");
	checkMoneyReserved(moneyReserved, minLoanPayment);
	currAge = get_valid_int("Enter your current age: ");
	retAge = get_valid_int("Enter the age you plan to retire at: ");
	retAge = checkRetAge(currAge, retAge, "Enter the age you plan to retire at: ");
	rateReturn = get_valid_double("Enter the annual rate of return you predict for your investments: ");
	//Calculate min loan total
	double minPay = calculateSavings(moneyReserved, moneyOwed, interestRate, rateReturn, currAge, retAge, minLoanPayment);
	//Calculate max loan total
	double maxPay = calculateSavings(moneyReserved, moneyOwed, interestRate, rateReturn, currAge, retAge, moneyReserved);
	//Compare and output
	output(minPay, maxPay, moneyReserved);
	return 0;
}