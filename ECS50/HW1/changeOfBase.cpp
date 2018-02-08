#include<iostream>
#include<cmath>
#include<cstdlib>
#include<string>
#include<algorithm>

using namespace std;

int base = 0,newBase = 0;
string num;

//Converts to base any number to base 10
int convertBase10(string numString, int base){
	int base10 = 0;
	int digitNum = 0; 
	//Multiply every number from right to left by the base to the power of its index
	for(int i = numString.length() - 1; i >= 0; i--){
		//Check if it is a number or letter
		int digit;
		if(isdigit(numString[i])){
			digit = numString[i] - 48;
		} else { //is a letter
			numString[i] = toupper(numString[i]);
			digit = numString[i] - 55;
		}
		if(digitNum == 0){
			base10 += digit;
		} else {
			base10 += digit * pow(base, digitNum);
		}
		digitNum++;
	}
	return base10;
}

//Converts from one base to another
string calculateNum(int base, string numString, int newBase){
	string newNum = "";
	int oldNumber = convertBase10(numString, base);
	int rem = 0;
	while(oldNumber != 0){
		if(oldNumber % newBase < 10){ //Add on number to string
			newNum += (oldNumber % newBase) + '0';
		} else { //Add on letter to string
			newNum += ((oldNumber % newBase) + 55);
		}
		oldNumber /= newBase;
		rem++;
	}
	//Flip the sting at the end
	reverse(newNum.begin(), newNum.end());
	return newNum;
}

int main(){
cout << "Please enter the number's base: ";
cin >> base;
cout << "Please enter the number: ";
cin >> num;
cout << "Please enter the new base: ";
cin >> newBase;
cout << num << " base " << base << " is " << calculateNum(base, num, newBase) << " base " << newBase << endl;
return 0;
}
