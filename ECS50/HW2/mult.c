#include<stdio.h>
#include<stdlib.h>

//Returns the binary representation of the product of 2 unsigned integers
unsigned long long mult(unsigned int n1, unsigned int n2){
	unsigned long long result = 0;
	unsigned long long a = (unsigned long long)n1; //Cast to avoid overflow
	int index;
	//Check each bit of n2
	for(index = 0; index < 32; index++){
		if(n2 & (1 << index)){ //If 1, then add n1 left shifted by that index
			result += (a << index);
		}
	}
	return result;
}

int main(int argc, char* argv[]){
	char *ptr;
	unsigned int n1 = strtoul(argv[1], &ptr, 10);
	unsigned int n2 = strtoul(argv[2], &ptr, 10);
	printf("%u * %u = %llu\n", n1, n2, mult(n1, n2));
	return 0;
}


