/*
Sergei Chestakov
ECS 30 HW 4
This program takes in 2 strings as input from the user and outputs whether or not those strings are anagrams of each other
*/
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

//Returns whether two words are anagrams of each other
bool isAnagram(char first[], char second[]){
	//They must be equal in length
	if (strlen(first) != strlen(second)){
		return false;
	}
	int i;
	int length = strlen(first);
	int total1 = 0, total2 = 0;
	for (i = 0; i < length; ++i){
		//tolower used for case insensitivity
		char lower1 = tolower(first[i]);
		char lower2 = tolower(second[i]);
		//Add all the characters up in both words to compare at the end
		total1 += lower1;
		total2 += lower2;
	}
	return total1 == total2;
}

//Outputs whether or not the two words are anagrams
void output(bool anagram, char first[], char second[]){
	if (anagram){
		printf("%s is an anagram of %s\n", first, second);
	} else {
		printf("%s is NOT an anagram of %s\n", first, second);
	}
}

int main(){
	char first[20];
	char second[20];
	//Prompt user for input
	printf("Please enter the first word: ");
	scanf("%s", first);
	printf("Please enter the second word: ");
	scanf("%s", second);
	//Test and output
	bool anagram = isAnagram(first, second);
	output(anagram, first, second);
	return 0;
}
