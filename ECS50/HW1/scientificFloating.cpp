#include<iostream>
#include<sstream>
#include<cmath>

using namespace std;

string mantissa(unsigned int fInt){
	stringstream s;
	//If 32nd bit is 1, then the number is negative
	if(fInt & (1 << 31)){
		s << "-";
	}
	s << "1.";
	int bit = 0;
	//Search for first 1 in mantissa because the initial zeroes are not printed
	int x = 1; 
	while((fInt & x) == 0 && bit < 23){
		x <<= 1;
		bit++;
	}
	//If mantissa field is all 0s
	if(bit == 23){return "0";}
	//Add every other bit to the string
	for(int i = 22; i >= bit; i--){
		if(fInt & (1 << i)){
			s << 1;
		} else {
			s << 0;
		}
	}
	return s.str();
}

int exponent(unsigned int fInt){
	int exp = 0;
	int p= 0;
	//Check bits 23 to 30 to find the exponent
	for(int bit = 23; bit < 31; bit++){
		if(fInt & (1 << bit)){
			exp += pow(2, p);
		}
		p++;
	}
	//If exponent field is all 0s
	if(exp == 0){return 0;}
	//Otherwise, substract the offset of 127 from the answer
	return exp - 127;
}

int main(){
	float input;
	cout << "Please enter a float: ";
	cin >> input;
	unsigned int float_int = *((unsigned int*)&input);
	cout << mantissa(float_int) << "E" << exponent(float_int);
}
