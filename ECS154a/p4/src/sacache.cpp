#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

unsigned short RAM [65536];

class CacheLine {

	unsigned short tag;
	bool dirty;
	unsigned short * memory;
	short count;

	public:

	CacheLine(){
		tag = -1;
		dirty = false;
		memory = new unsigned short [4];
		count = 0;
	}

	~CacheLine(){
		delete[] memory;
	}

	unsigned short getTag(){
		return tag;
	}

	short getCount(){
		return count;
	}

	void increment(){
		count++;
	}
	
	void updateMemory(unsigned short prevTag, unsigned short set){
		if(dirty){
			for(short i = 0; i < 8; i++){
				unsigned short memAddr = (tag << 8) | (set << 3) | i;
				RAM[memAddr] = memory[i];
			}
		}		
		//Bring in the correct line from memory
		tag = prevTag;
		for(short i = 0; i < 8; i++){
			unsigned short memAddr = (tag << 8) | (set << 3) | i;
			memory[i] = RAM[memAddr];
		}
	}

	//If the tag is there just return the number at the offset, else bring in the entire line from RAM and return
	void read(unsigned short readTag, unsigned short set, unsigned short offset, ofstream& outFile){
		bool oldDirty = dirty;
		bool hit = true;
		if(readTag != tag){
			//Update memory
			updateMemory(readTag, set);
			dirty = false;
			hit = false;
		} 
		count = 0;
		//Output
		outFile << hex << uppercase << setw(2) << setfill('0');
		outFile << memory[offset] << " " << hit << " " << oldDirty << endl;
	}

	void write(unsigned short writeTag, unsigned short set, unsigned short offset, unsigned short data){
		if(writeTag != tag){
			updateMemory(writeTag, set);
		}
		count = 0; 
		tag = writeTag;
		memory[offset] = data;
		dirty = true;
	}
};

int main(int argc, char* argv[]){
	ifstream inFile(argv[1]);
	ofstream outFile("sa-out.txt");
	CacheLine cache [16][8];
	unsigned short addr, op, data;
	while(inFile >> hex >> addr >> op >> data){
		//Extract from address
		unsigned short offset = addr & 3;
		unsigned short set = (addr >> 2) & (15);
		unsigned short tag = addr >> 6;

		short maxCount = 0;
		short maxIndex = 0;
		bool found = false;
		//Search for tag in set, if it is not there use LRU
		for(short i = 0; i < 8; i++){			
			cache[set][i].increment();
			if(cache[set][i].getTag() == tag){
				if(op & 1){
					cache[set][i].write(tag, set, offset, data);
				} else {
					cache[set][i].read(tag, set, offset, outFile);
				}
				found = true;
			} else { //Look for LRU
				if(cache[set][i].getCount() > maxCount){
					maxCount = cache[set][i].getCount();
					maxIndex = i;
				}
			}
		}
		if(!found){ //Evict LRU
			if(op & 1){
				cache[set][maxIndex].write(tag, set, offset, data);				
			} else {
				cache[set][maxIndex].read(tag, set, offset, outFile);
			}
		}
	}
	inFile.close();
	outFile.close();
}
