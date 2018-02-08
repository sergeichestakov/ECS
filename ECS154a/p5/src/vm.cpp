#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

class RAM {

	const static short RAM_SIZE = 4;
	unsigned int * memory;
	bool * useBits;
	short clock;

	public:

	RAM(){
		memory = new unsigned int[RAM_SIZE];
		useBits = new bool[RAM_SIZE];
		clock = 0;
	}

	~RAM(){
		delete[] memory;
		delete[] useBits;
	}

	void insert(unsigned int addr){
		addr >>= 10;
		//Check it it is there, if not move clock until you find a zero, replacing everything along the way
		for(short i = 0; i < RAM_SIZE; i++){
			if(memory[i] == addr){
				useBits[i] = true;
				return;
			}
		}
		while(useBits[clock]){
			useBits[clock] = false;
			clock == RAM_SIZE - 1 ? clock = 0 : clock++;
		}
		memory[clock] = addr;
		useBits[clock] = true;
		clock == RAM_SIZE - 1 ? clock = 0 : clock++;
	}

	void output(ofstream& out){
		for(short i = 0; i < RAM_SIZE; i++){
			unsigned int addr = memory[i];
			if(addr){
				addr <<= 10;
				out << hex << addr;
				if(i != RAM_SIZE-1 && memory[i+1]){out << " ";}
			}
		}
		out << endl;
	}

	
};

int main(int argc, char* argv[]){
	ifstream inFile(argv[1]);
	ofstream outFile("vm-out.txt");
	unsigned int addr;
	for(short i = 0; i < 32; i++){
		inFile >> hex >> addr;
	}
	RAM ram;
	while(inFile >> hex >> addr){
		ram.insert(addr);
		ram.output(outFile);
	}
	inFile.close();
	outFile.close();
}
