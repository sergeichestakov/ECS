/*
 * Sergei Chestakov
 * ECS 30
 * This program converts either temperature (Farenheit, Celcius, or Miles) or distance(Inches, Feet, Yards, and Miles) depending on user input
 */
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>

//Checks to make sure correct number of arguments are read
void assertFormat(int numArgsRead, int numArgsNeeded){
  //Also checks to make sure that the character following the recieved input is a new line to ensure nothing else was entered after the recieved input
  char next;
  scanf("%c", &next);
  if (numArgsRead != numArgsNeeded || next != '\n'){
    printf("Invalid formatting. Ending program.\n");
    exit(0);
  }
}

//Checks to make sure the inputted character was a valid unit that can be converted in our program
void validType(char directions, char unitType){
   if (directions == 't' || directions == 'T'){
     if (unitType != 'f' && unitType != 'F' && unitType != 'c' && unitType != 'C' &&
       unitType != 'k' && unitType != 'K'){
       printf("%c is not a valid temperature type. Ending program.\n", unitType);
       exit(0);
     }
     } else {
      if(unitType != 'i' && unitType != 'I' && unitType != 'f' && unitType != 'F' &&
	unitType != 'y' && unitType != 'Y' && unitType != 'm' && unitType != 'M') {
	printf("%c is not a valid distance type. Ending program.\n", unitType);
	exit(0);
     }
     }
}

//Determines whether to convert temperature or distance, or if the input was invalid
bool checkConversion(char directions){
  switch(directions){
    case 't':
    case 'T': return true;
    case 'd':
    case 'D': return false;
    default: 
      printf("Unknown conversion type %c chosen. Ending Program.\n", directions);
      exit(0);
  }
}

//Temperature Conversions, centered around Celcius
double KtoC(double k){
  return k - 273.15;
}

double FtoC(double f){
  return (f - 32) * (5.0/9.0);
}

double CtoK(double c){
  return c + 273.15;
}

double CtoF(double c){
  return (c * (9.0/5.0)) + 32.0;
}

//Distance conversions, centered around Feet
double ItoF(double i){
  return i/12.0;
}

double YtoF(double y){
  return y * 3.0;
}

double MtoF(double m){
  return m * 5280.0;
}

double FtoI(double f){
  return f * 12.0;
}

double FtoY(double f){
  return f / 3.0;
}

double FtoM(double f){
  return f / 5280.0;
}

//Converts the temperature given the temp, original unit, and desired unit
double convertTemp(double originalTemp, char origUnit, char newUnit){
  if (toupper(origUnit) == toupper(newUnit)){
    return originalTemp;
  }
  if (origUnit == 'c' || origUnit == 'C'){
    if (newUnit == 'f' || newUnit == 'F'){
      return CtoF(originalTemp);
    } else {
      return CtoK(originalTemp);
    }
  }
  else if (origUnit == 'f' || origUnit == 'F'){
    if (newUnit == 'c' || newUnit == 'C'){
      return FtoC(originalTemp);
    } else {
      return CtoK(FtoC(originalTemp));
    }
  }
  else {
    if (newUnit == 'c' || newUnit == 'C'){
      return KtoC(originalTemp);
    } else {
      return CtoF(KtoC(originalTemp));
    }
  }
}

//Converts the distance to the desired unit
double convertDist(double originalDist, char origUnit, char newUnit){
  if (toupper(origUnit) == toupper(newUnit)){
    return originalDist;
  }
  if (origUnit == 'i' || origUnit == 'I'){
    switch(newUnit){
      case 'f':
      case 'F': return ItoF(originalDist);
      case 'y':
      case 'Y': return FtoY(ItoF(originalDist));
      default: return FtoM(ItoF(originalDist));
    }
  }
  else if (origUnit == 'f' || origUnit == 'F'){
    switch(newUnit){
      case 'i':
      case 'I': return FtoI(originalDist);
      case 'y':
      case 'Y': return FtoY(originalDist);
      default: return FtoM(originalDist);
    }
  }
  else if (origUnit == 'y' || origUnit == 'Y'){
    switch(newUnit){
      case 'i':
      case 'I': return FtoI(YtoF(originalDist));
      case 'f':
      case 'F': return YtoF(originalDist);
      default: return FtoM(YtoF(originalDist));
    }
  }
  else{
    switch(newUnit){
      case 'i':
      case 'I': return FtoI(MtoF(originalDist));
      case 'f':
      case 'F': return MtoF(originalDist);
      default: return FtoY(MtoF(originalDist));
    }
  }
}

//Figure out which conversion to do and output the correct prompt and calculations based on the input
void run(bool tempConversion, char directions){
  //Prompt and output for temperature
  if (tempConversion){
    double temp;
    char originalUnit, newUnit;
    printf("Enter the temperature followed by its suffix (F, C, or K): ");
    int numArgsRead = scanf(" %lf %c", &temp, &originalUnit);
    assertFormat(numArgsRead, 2);
    //Make sure unit is valid
    validType(directions, originalUnit);
    printf("\nEnter the new unit type (F, C, or K): ");
    numArgsRead = scanf(" %c", &newUnit);
    //Make sure unit is valid
    assertFormat(numArgsRead, 1);
    validType(directions, newUnit);
    //Convert
    double finalTemp = convertTemp(temp, originalUnit, newUnit);
    //Output
    printf("%.2lf%c is %.2lf%c\n", temp, toupper(originalUnit), finalTemp, toupper(newUnit));
  } else {
    //Prompt and output for distance
    double distance;
    char originalUnit, newUnit;
    printf("Enter the distance followed by its suffix (I,F,Y,M): ");
    int numArgsRead = scanf(" %lf %c", &distance, &originalUnit);
    assertFormat(numArgsRead, 2);
    //Make sure unit is valid
    validType(directions, originalUnit);
    printf("\nEnter the new unit type (I,F,Y,M): ");
    numArgsRead = scanf(" %c", &newUnit);
    //Make sure unit is valid
    assertFormat(numArgsRead, 1);
    validType(directions, newUnit);
    //Convert
    double finalDist = convertDist(distance, originalUnit, newUnit);
    //Output
    printf("%.2lf%c is %.2lf%c\n", distance, toupper(originalUnit), finalDist, toupper(newUnit));
  }
}

int main(){
  char directions;
  //Prompt user which conversion type they want
  printf("Pick the type of conversion that you would like to do.\n");
  printf("T or t for temperature\nD or d for distance\n");
  printf("Enter your choice: ");
  int numArgsRead = scanf(" %c", &directions);
  //Assert format after each scan to ensure valid input
  assertFormat(numArgsRead, 1);
  //Determine conversion
  bool convertTemp = checkConversion(directions);
  //Run program based on input
  run(convertTemp, directions);
  return 0; 
}
