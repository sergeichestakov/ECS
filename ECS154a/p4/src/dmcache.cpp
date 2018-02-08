#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

unsigned short RAM [65536];

class CacheLine {

	unsigned short tag;
	bool dirty;
	unsigned short * memory;

	public:

	CacheLine(){
		tag = -1;
		dirty = false;
		memory = new unsigned short [8];
	}

	~CacheLine(){
		delete[] memory;
	}

	void updateMemory(unsigned short prevTag, unsigned short line){
		if(dirty){
			for(short i = 0; i < 8; i++){
				unsigned short memAddr = (tag << 8) | (line << 3) | i;
				RAM[memAddr] = memory[i];
			}
		}		
		//Bring in the correct line from memory
		tag = prevTag;
		for(short i = 0; i < 8; i++){
			unsigned short memAddr = (tag << 8) | (line << 3) | i;
			memory[i] = RAM[memAddr];
		}
	}
	
	//If tag is the same then return data at that offset
	//Else it is a readmiss so set dirty bit to 0 and replace tag and data
	void read(unsigned short readTag, unsigned short line, unsigned short offset, ofstream& outFile){
		bool oldDirty = dirty;
		bool hit = true;
		if(readTag != tag){
			//Update memory
			updateMemory(readTag, line);
			dirty = false;
			hit = false;
		} 
		//Output
		outFile << hex << uppercase << setw(2) << setfill('0');
		outFile << memory[offset] << " " << hit << " " << oldDirty << endl;
	}

	void write(unsigned short writeTag, unsigned short line, unsigned short offset, unsigned short data){
		if(writeTag != tag){
			updateMemory(writeTag, line);
		}
		//Insert data and set dirty bit
		memory[offset] = data;
		dirty = true;
	}
};

int main(int argc, char* argv[]){
	ifstream inFile(argv[1]);
	ofstream outFile("dm-out.txt");
	CacheLine cache [32];
	unsigned short addr, op, data;
	while(inFile >> hex >> addr >> op >> data){
		//Extract from address
		unsigned short offset = addr & 7;
		unsigned short line = (addr >> 3) & (31);
		unsigned short tag = addr >> 8;
		if(op & 1){//Write
			cache[line].write(tag, line, offset, data);			
		} else { //Read
			cache[line].read(tag, line, offset, outFile);
		}
	}
	inFile.close();
	outFile.close();
}
