/*
Sergei Chestakov
ECS 30
This function adds numbers recursively and outputs the result
*/ 
#include <stdio.h>
#include <stdlib.h>

int _foo(int num, int n1, int n2, int n3){
	if(num == 0){
		return n3;
	}
	else if(num == 1){
		return n2;
	}
	else if(num == 2){
		return n1;
	}
	else {
		return _foo(num - 1, n1 + n2 + n3, n1, n2);
	}
}

int foo(int num){
	return _foo(num, 5, 3, 2);
}

int main(int argc, char* argv[]){
	int num = atoi(argv[1]);
	int fooNum = foo(num);
	printf("foo(%d) = %d\n", num, fooNum);
	return 0;
}
