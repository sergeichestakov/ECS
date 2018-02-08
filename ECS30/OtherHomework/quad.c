/*
 * Sergei Chestakov
 * ECS 30
 * This program calculates the solutions to an inputed quadratic equation using the quadratic formula
 */
#include<stdio.h>
#include<math.h>

//Finds the positive root using the quadratic formula
double findPositiveRoot(double a, double b, double c){
  return (-b + sqrt(b*b - 4*a*c))/(2*a);
}

//Finds the negative root using the quadratic formula
double findNegativeRoot(double a, double b, double c){
  return (-b - sqrt(b*b - 4*a*c))/(2*a);
}

//Outputs the solution or states there are none depending on the inputed values
void output(double a, double b, double c){
  double determinant = b*b - (4*a*c);
  double root1 = findPositiveRoot(a,b,c);
  double root2 = findNegativeRoot(a,b,c);
  //Negative determinant means there are no real solutions because all solutions must contain imaginary numbers
  if (determinant < 0){
    printf("There are no real solutions\n");
  }
  //If the roots are the same then there is only one real solution
  else if (fabs(root1 - root2) < 0.0001){
    printf("There is one real solution: %.2lf\n", root1);
  } 
  //Otherwise print the two real solutions
  else {
    printf("There are 2 real solutions\n");
    printf("Solution 1: %.2lf\n", root1);
    printf("Solution 2: %.2lf\n", root2);
  }
  
}

int main(){
  double a,b,c;
  //Prompt user for input
  printf("Given a quadratic equation of the form a*x^2 + b * x + c\n");
  printf("Please enter a: ");
  scanf("%lf", &a);
  printf("\nPlease enter b: ");
  scanf("%lf", &b);
  printf("\nPlease enter c: ");
  scanf("%lf", &c);
  output(a,b,c);
  return 0;
}