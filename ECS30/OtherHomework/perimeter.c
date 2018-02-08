/*
Sergei Chestakov
ECS 30
This program reads a specified amount of integer points from a binary file and returns the perimeter of the polygon formed around those points
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//Distance formula
double distance(int x1, int y1, int x2, int y2){
	return sqrt(abs(pow((x2 - x1), 2)) + abs(pow((y2 - y1), 2)));
}

int main(int argc, char* argv[]){
	FILE* fp = fopen(argv[1], "rb");
	int numPoints;
	//Read in how many points are going to be read in
	fread(&numPoints, sizeof(int), 1, fp);
	double perimeter = 0;
	int x1, y1, x2, y2;
	//Read in the first point
	fread(&x1, sizeof(int), 1, fp);
	fread(&y1, sizeof(int), 1, fp);
	//Save the first point to be used at the end
	int firstX = x1;
	int firstY = y1;
	int point;
	for(point = 0; point < numPoints - 1; ++point){
		//Read in next point
		fread(&x2, sizeof(int), 1, fp);
		fread(&y2, sizeof(int), 1, fp);	
		//Add the distance between the two points to the perimeter
		perimeter += distance(x1, y1, x2, y2);
		//Set the second points values to the first
		x1 = x2;
		y1 = y2;
	}
	//Add the distance between first and last points
	perimeter += distance(x2, y2, firstX, firstY);
	//Output
	printf("The perimeter is %.2lf\n", perimeter);
	return 0;
}
