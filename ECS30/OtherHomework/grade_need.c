/*
 * Sergei Chestakov
 * ECS 30
 * This program calculates the score the user needs to recieve on the final exam given the letter grade they want, the percent they currently have in the class, and the weight of the final
 */

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

//Returns the percentage needed to recieve each grade
double calculatePercent(char letterGrade){
  switch(letterGrade){
    case 'a':
    case 'A': return 90;
    case 'b':
    case 'B': return 80;
    case 'c':
    case 'C': return 70;
    case 'd':
    case 'D': return 60;
    case 'f':
    case 'F': return 0;
    //If grade inputted is not one of the above, display an error and terminate
    default: printf("Unknown Grade Received: %c. Ending program.\n", letterGrade);
    exit(0);
  }
}

//Ensures user has inputted correctly by checking the number of arguments read against the num arguments expected, if not then terminates the program
void assertFormat(int numArgsRead, int numArgsNeeded, double num){
  //Also checks to make sure that the character following the recieved input is a new line
  char next;
  scanf("%c", &next);
  if (numArgsRead != numArgsNeeded || next != '\n'){
    printf("Invalid formatting. Ending program.\n");
    exit(0);
  }
  //If number entered is negative, display a different error and terminate
  if (num < 0){
    printf("The number you last entered should have been positive. Ending program.\n");
    exit(0);
  }
}

//Calculate the final score based on the user inputted values
double calculateFinalScore(double currentPercent, double finalWeight, double percentNeeded){
  double currentPoints = currentPercent * ((100-finalWeight)/100);
  double pointsNeeded = percentNeeded - currentPoints;
  double finalScore = (pointsNeeded/finalWeight) * 100;
  return finalScore;
}

//Output based on the final score
void output(double finalScore, char letterGrade){
  //Final cannot be more than 100% or less than 0%
  if (finalScore > 100){
    printf("You cannot get a %c in the class because you would need to score at least %.2lf%% on the final.\n", letterGrade, finalScore);
  }
  else if (finalScore < 0){
    printf("You cannot score low enough on the final to get a %c in the class.\n", letterGrade);
  }
  else {
    printf("\nYou need a grade of at least %.2f%% on the final to get a %c in the class.\n", finalScore, letterGrade);
  }
}

int main(){
 //Declare variables
  char letterGrade;
  double currentPercent, finalWeight;
  //Accept inputs 
  printf("Enter the grade you want in the class: ");
  //Keep track of the number of arguments scanned
  int numArgs = scanf("%c", &letterGrade);
  //Assert format after each input to insure correct format or terminate the program immediately
  assertFormat(numArgs, 1, 0);
  double percentNeeded = calculatePercent(letterGrade);
  printf("\nEnter your current percent in the class: ");
  int numArgs2 = scanf(" %lf", &currentPercent);
  assertFormat(numArgs2, 1, currentPercent);
  printf("\nEnter the weight of the final: ");
  int numArgs3 = scanf(" %lf", &finalWeight);
  assertFormat(numArgs3, 1, finalWeight);
  //Calculate the percent they need on final
  double finalScore = calculateFinalScore(currentPercent, finalWeight, percentNeeded);
  output(finalScore, letterGrade);
  return 0;
}