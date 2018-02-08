#include <cmath>
#include "cpu.h"

//Inefficient/original backup located in cpu.1.cpp

CPU::CPU() {
}

//right now it doesn't/double check that it works with random             and doesn't try to use the incorrectly loaded thing

//maybe later set it up so that you don't have to clear addresses you've passed, then you would need to check addresses, this would make this a kind of hybrid hash table

//array of things that could be sums or individual numbers (which may not have been loaded)

class Value {
  public:
    Value(int val) : val(val) {}
    virtual bool dependsOn(int index) { return false; }
    virtual bool inRAM() { return true; }
    
    int val;
}

class Sum : class Value {
  public:
    Sum(int index1, int index2) : index1(index1), index2(index2) {}
    bool dependsOn(int index) { return index1 == index || index2 == index; }
    bool inRAM() { return false; }
    
    int index1;
    int index2;
}

Instruction currInstruction;
const int LENGTH = 2049;
unsigned minAddress;
bool passed = false; //minAddress has been changes so we've now started overwriting
int minIndex = -1; //from 0, set to LENGTH/2 initialliy
Value* sums[LENGTH] = {NULL}; //only contains the store to addresses?? - no, not true, increase this size to max later
//store when about to/already fall off or be overwritten with another address

int temp1;
int temp2;


void CPU::operation(OpCode &opCode, Instruction &instruction, int &buffer) {
  switch (opCode) {
    //*addr3 = *addr1 + *addr2
    case ADD:
      //store instruction in case, it takes multiple loops to process
      currInstruction = instruction;
      add(opCode, instruction, buffer);
      break;
    
    //Buffer contains int requested from RAM
    case LOAD:
      insertValue(instruction, buffer)
      add(opCode, instruction, buffer);
      break;
    
    //Sent by RAM after a STORE
    case STORE:
      add(opCode, instruction, buffer);
      break;
    
    //All ADDs in file have been sent.  Time to STORE dirty cache.
    case DONE:
      break;
  }
}

void CPU::slowAdd(OpCode &opCode, Instruction &instruction, int &buffer) {
  //TODO but for now, blank so we know if the rest runs without seg fault
}

void CPU::add(OpCode &opCode, Instruction &instruction, int &buffer) {
  //too long of range, must be random add   -- could prob be optimized
  if(!inRange(instruction)) {
    slowAdd(opCode, instruction, buffer);
    return;
  }
  
  //first add
  if(minIndex == -1) {
    minIndex = LENGTH/2;
    minAddress = instruction.addr3;
    //TODO looping around if needed
    int index1 = (minIndex - minAddress + instruction.addr1) % (LENGTH-1);
    int index2 = (minIndex - minAddress + instruction.addr2) % (LENGTH-1);
    sums[minIndex] = new Sum(index1, index2);
  } else {
    int sumIndex = (minIndex - minAddress + instruction.addr3) % (LENGTH-1);
    
    //check if new min           PROBLEM we never go back so random fucks it up and it never gets fices after that. also if a new process is earlier, it gets screwed up - maybe go back after 10 bellow
    if(!passed )

    //if we're passing
    if(instruction.addr3 > minAddress) {
      //remove overlapping and set to null
      for(int i=minIndex;i<=sumIndex;i++) {
        if(sums[i] && sums[i]->inRAM() == false) {
          storeToRAM(opCode, instruction, buffer, i);
          return;
        }
        //NOT DONE SET NEW MIN addr and index, which addr is higher
        //store if needed, set to null
      }
      passed = true;
    }
    
    //handle case if overwriting something
    if(sums[sumIndex] != NULL) {
      //TODO: check if anything actually changes, break if so
      //if() {}
      
      //store all things that were referencing what is about to be changed
      for(int i=0;i<LENGTH;i++) {
        if(sums[i] && sums[i]->dependsOn(sumIndex) && i != sumIndex) {
          storeToRAM(opCode, instruction, buffer, i);
          return;
        }
      }
      
      //no conflicts, overwrite/update
      delete sums[sumIndex];
      insertSum(instruction, sumIndex);
    } else {
      insertSum(instruction, sumIndex);
    }
  }
}

bool CPU::inRange(unsigned addr1, unsigned addr2, unsigned addr3) {
  unsigned min;
  unsigned max;
  if(addr1 >= addr2 && addr1 >= addr3) {
    max = addr1;
    if(addr2 <= addr3) {
      min = addr2;
    } else {
      min = addr3;
    }
  } else if(addr2 >= addr1 && addr2 >= addr3) {
    max = addr2;
    if(addr1 <= addr3) {
      min = addr1;
    } else {
      min = addr3;
    }
  } else {
    max = addr3;
    if(addr1 <= addr2) {
      min = addr1;
    } else {
      min = addr2;
    }
  }
  return max-min < LENGTH;
}

unsigned CPU::calculateAddr(int index) {
  //TODO
}

void CPU::insertValue(unsigned address, int value) {
  //TODO
}

void CPU::insertSum(Instruction &instruction, int sumIndex) {
  //TODO: wrap around index
    int index1 = (minIndex - minAddress + instruction.addr1) % (LENGTH-1);
    int index2 = (minIndex - minAddress + instruction.addr2) % (LENGTH-1);
    sums[sumIndex] = new Sum(index1, index2);
}

//assumes that where we are loading to is null
void CPU::loadFromRAM(OpCode &opCode, Instruction &instruction, int index) {
  opCode = LOAD;
  instruction.addr1 = calculateAddr(index);
}

void CPU::storeToRAM(OpCode &opCode, Instruction &instruction, int &buffer, int index) {
  //store all things that were referencing what is about to be changed
  for(int i=0;i<LENGTH;i++) {
    if(sums[i] && sums[i]->dependsOn(index) && i != index) {
      storeToRAM(opCode, instruction, buffer, i);
      return;
    }
  }
  
  //load index1 and index2 if not already there
  int index1 = sums[sumIndex]->index1;
  int index2 = sums[sumIndex]->index2;
  //If they're out of range -- this section can be optimized by saving one of the addresses' value if that one is in range
  if((passed && calculateAddr(index1) < minAddress) || (passed && calculateAddr(index2) < minAddress)) {
    currInstruction.addr1 = calculateAddr(index1);
    currInstruction.addr2 = calculateAddr(index2);
    currInstruction.addr3 = calculateAddr(sumIndex);
    delete sums[sumIndex]
    slowAdd(opCode, instruction buffer);
    return;
  }
  //load index1 and index2 if not already there and in range
  if(sums[index1] == NULL) {
    loadFromRAM(opCode, instruction, index1);
    return;
  }
  if(sums[index2] == NULL) {
    loadFromRAM(opCode, instruction, index2);
    return;
  }
  
  //done, so store this
  opCode = STORE;
  instruction.addr1 = calculateAddr(index);
  int val1 = sums[index1]->val;
  int val2 = sums[index2]->val;
  buffer = val1+val2;
  
  //Update
  delete sums[sumIndex];
  sums[sumIndex] = new Value(buffer);
}