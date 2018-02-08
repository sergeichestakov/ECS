/*
Sergei Chestakov
ECS 30
This program calculates the best combination of items to steal from a jewelry store based on the inputted price and values of each item and how much the user can hold in their backpacks
*/
#include <stdio.h>
#include <stdlib.h>

//Each item has a value and weight
typedef struct Item_struct {
	int value;
	int weight;
} Item;

void recieveInput(int* weight, int* numItems);
Item* getItems(int numItems);
void swap(Item* list, int i, int j);
void findMaxVal(Item* list, int numItems, int currIndex, int maxWeight, int* maxValue);

int main(){
	int weight, numItems;
	recieveInput(&weight, &numItems);
	Item* list = getItems(numItems);
	int mostProfit = 0;
	findMaxVal(list, numItems, 0, weight, &mostProfit);
	printf("They most profitable heist will net you $%d in total.\n", mostProfit);
	return 0;
}

//Swap 2 items in a list
void swap(Item* list, int i, int j){
	Item temp = list[i];
	list[i] = list[j];
	list[j] = temp;
}

//This function generates all possible permutations of the list of items and adds up their valid respective values
void findMaxVal(Item* list, int numItems, int currIndex, int maxWeight, int* maxValue){
	int j;
	//Base case: if the current index is equal to the number of items
	if(currIndex == numItems){
		int value = 0, weight = 0;
		//Count up the valid value
		for(j = 0; j < numItems; j++){
			weight += list[j].weight;
			//Check to make sure the weight is less than the weight your backpack can hold before adding the value of the item, if not break the loop and move on to the next permutation
			if(weight < maxWeight){
				value += list[j].value;
			} else {
				break;
			}
		}
		//Check each permutation and update the maximum value accordingly
		if(value > *maxValue){
			*maxValue = value;
		}
	} else {
		//Recursive case: swap each index and find the permutation of all smaller lists
		for(j = currIndex; j < numItems; j++){
			swap(list, currIndex, j);
			findMaxVal(list, numItems, currIndex + 1, maxWeight, maxValue);
			swap(list, currIndex, j);
		}
	}
}

//Prompt the user and scan in values
void recieveInput(int* weight, int* numItems){ 
	printf("Enter the maximum amount of weight that your bag can hold: ");
	scanf("%d", weight);
	printf("Enter the number of items that are in the jewelry store: ");
	scanf("%d", numItems);
	printf("Enter your items, one per line (value weight):\n");
}

//Get each of the items' values and weight and return it as an array of Item objects
Item* getItems(int numItems){
	Item* items = (Item*)malloc(numItems * sizeof(Item));
	int item;
	for(item = 0; item < numItems; item++){
		scanf("%d", &items[item].value);
		scanf("%d", &items[item].weight);
	}
	return items;
}
