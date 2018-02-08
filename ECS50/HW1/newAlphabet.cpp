#include<iostream>
#include<cstdlib>

using namespace std;

int main(int argc, char*argv[]){
	cout << "You entered the word: "; 
	for(int letter = 1; letter < argc; letter++){
		int num = atoi(argv[letter]);
		unsigned int numLetter = (unsigned int)num;
		int count = 0;
		bool capital = false;
		//If 27th bit is a 1 its a capital letter
		if((numLetter & (1 << 26))){
			capital = true;
		}
		//While the first bit is not a 1, keep incrementing and shifting the number over
		while((numLetter & 1) == 0){
			numLetter >>= 1;
			count++;
		}
		//Print the corresponding letter
		if(capital){
			cout << (char)(count + 65);
		} else {
			cout << (char)(count + 97);
		}
	}
}
