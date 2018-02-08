/*
 * Sergei Chestakov
 * ECS 30
 * This program multiplies 2 inputed complex numbers together and outputs the result
 */
#include <stdio.h>

int main(){
  //declare variables for the imaginary and real parts of both complex numbers
  int a1, b1, a2, b2;
  //input complex numbers to give values to variables
  printf("Enter the first complex number in the form ai + b: ");
  scanf("%d i + %d", &a1, &b1);
  printf("\nEnter the second complex number in the form ai + b: ");
  scanf("%d i + %d", &a2, &b2);
  //Multiply out to find both imaginary and real numbers using FOIL method
  int a3 = (a1 * b2) + (a2 * b1);
  int b3 = (b1 * b2) - (a1 * a2);
  //Output result
  printf("\n(%di + %d) * (%di + %d) = %di + %d\n", a1, b1, a2, b2, a3, b3);
  return 0;
}  