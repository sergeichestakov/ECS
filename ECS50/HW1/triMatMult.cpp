#include<iostream>
#include<fstream>
#include<vector>

using namespace std;

//Create a vector of all of the values read in from a file
vector<int> createVector(ifstream & in){
	vector<int> v;
	int value;
	while(in >> value){
		v.push_back(value);
	}
	return v;
}

//Return the number of zeroes in each row in order to find the correct index in the compressed matrix
int numZeroes(int row){
	return (row * (row + 1))/2;
}

int main(int argc, char* argv[]){
	ifstream file1(argv[1]);
	ifstream file2(argv[2]);
	int n1, n2;
	//Read in from files and store values in a vector
	file1 >> n1;
	file2 >> n2;
	vector<int> v1 = createVector(file1);
	vector<int> v2 = createVector(file2);
	//Dot every row in n1 against every column in n2 and output result
	int result;
	for(int row1 = 0; row1 < n1; row1++){
		for(int col2 = 0; col2 < n2; col2++){
			//Start at the same column as each row
			int col1 = row1;
			if(col1 > col2){continue;}
			result = 0;
			//Dot product
			while(!(col1 > col2)){
				//Column of first mat == row of second mat
				result += (v1.at(row1*n1 + col1 - numZeroes(row1)) * v2.at(col1*n2 + col2 - numZeroes(col1)));
				col1++;
			}
			//Print out resulting value
			cout << result << " ";
		}
		
	}
	cout << endl;
}
