/*
 * Sergei Chestakov
 * ECS 30
 * HW 1.1
 * This program calculates the monthly payment of a loan as well as the total cost of the loan given the principal, interest rate, and number of payments and outputs it to the user
 */
#include <stdio.h>
#include <math.h>

int main(){
  //Declare variables for all inputs needed 
  double principal, interestRate, monthlyPayment;
  int numPayments;
  //Ask user to input values for each variable
  printf("Please enter the amount of money you borrowed: $");
  scanf("%lf", &principal);
  printf("Please enter the annual interest rate: ");
  scanf("%lf", &interestRate);
  printf("Please enter the number of payments to be made: ");
  scanf("%d", &numPayments);
  //Calculates monthly payment using given formula and monthly interest rate which is 1/12 of the inputed annual interest rate
  monthlyPayment = (interestRate/12 * principal) / (1 - pow(1 + interestRate/12, -numPayments));
  //Output results
  printf("A loan of $%.2lf with an annual interest of %.2lf payed off over %d months will have monthly payments of $%.2lf. ", principal, interestRate, numPayments, monthlyPayment); 
  //Total payment of the loan
  double totalPayment = monthlyPayment * numPayments;
  printf("In total you will pay $%.2lf, making the cost of your loan $%.2lf.\n", totalPayment, totalPayment - principal);
  return 0;
}