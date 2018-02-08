#include "cpu.h"
#include <iostream> //REMOVE

//Inefficient/original backup located in cpu.1.cpp

CPU::CPU() {
}

//right now, assuming addresses of different ints are one apart, will need to do some 4 multiplyers all over if not

//if errors, look at the mixing of signed and unsigned numbers --- also check if you can use sumIndex throughout like that, does it need to be updated after minIndex changes? - i don't think so but double check

//right now it doesn't/double check that it works with random             and doesn't try to use the incorrectly loaded thing

//maybe later set it up so that you don't have to clear addresses you've passed, then you would need to check addresses, this would make this a kind of hybrid hash table

//array of things that could be sums or individual numbers (which may not have been loaded)

class Item {
  public:
    virtual bool dependsOn(int index) = 0;
    virtual bool inRAM() = 0;
    virtual ~Item() {};
};

class Value : public Item {
  public:
    Value(int val) : val(val) {}
    virtual bool dependsOn(int index) { return false; }
    virtual bool inRAM() { return true; }
    
    int val;
};

class Sum : public Item {
  public:
    Sum(int index1, int index2) : index1(index1), index2(index2) {}
    bool dependsOn(int index) { return index1 == index || index2 == index; }
    bool inRAM() { return false; }
    
    int index1;
    int index2;
};

Instruction currInstruction;
const int LENGTH = 2049;
int beforeMinCount = 0;
int afterMaxCount = 0;
unsigned minAddress;
bool passed = false; //minAddress has been changes so we've now started overwriting
int minIndex = -1; //from 0, set to LENGTH/2 initialliy
//Item **sums = new Item*[LENGTH]; //only contains the store to addresses?? - no, not true, increase this size to max later //FIX SO THAT IT USES NEW
Item *sums[LENGTH] = {0}; //only contains the store to addresses?? - no, not true, increase this size to max later //FIX SO THAT IT USES NEW
//store when about to/already fall off or be overwritten with another address

Instruction slowInstruction;
int temp1;
int temp2;
bool haveTemp1 = false;
bool haveTemp2 = false;
bool slowed = false;

bool done = false;

int count = 0;

void CPU::operation(OpCode &opCode, Instruction &instruction, int &buffer) {
  switch (opCode) {
    //*addr3 = *addr1 + *addr2
    case ADD:
      count++;
      //std::cout << count << std::endl;
      //store instruction in case, it takes multiple loops to process
      currInstruction = instruction;
      add(opCode, instruction, buffer);
      break;
    
    //Buffer contains int requested from RAM
    case LOAD:
      if(slowed) {
        if(!haveTemp1) {
          temp1 = buffer;
          haveTemp1 = true;
        } else {
          temp2 = buffer;
          haveTemp2 = true;
        }
        slowAdd(opCode, instruction, buffer);
        return;
      }
      insertValue(instruction.addr1, buffer);
      if(done) {
        if(resetRange(opCode, instruction, buffer, 0, LENGTH-1)) {
          opCode = DONE;
          return;
        }
        return;
      }
      add(opCode, instruction, buffer);
      break;
    
    //Sent by RAM after a STORE
    case STORE:
      if(done) {
        slowed = false;
        if(resetRange(opCode, instruction, buffer, 0, LENGTH-1)) {
          opCode = DONE;
          return;
        }
      }
      if(slowed) {
        slowed = false;
        opCode = ADD;
        return;
      }
      add(opCode, instruction, buffer);
      break;
    
    //All ADDs in file have been sent.  Time to STORE dirty cache.
    case DONE:
      done = true;
      if(resetRange(opCode, instruction, buffer, 0, LENGTH-1)) {
        opCode = DONE;
        return;
      }
      break;
  }
}

void CPU::slowAdd(OpCode &opCode, Instruction &instruction, int &buffer) {
  slowed = true;
  
  //get values at addresses 1 and 2
  if(!haveTemp1) {
    opCode = LOAD;
    instruction.addr1 = slowInstruction.addr1;
    return;
  }
  if(!haveTemp2) {
    opCode = LOAD;
    instruction.addr1 = slowInstruction.addr2;
    return;
  }

  //add
  haveTemp1 = false;
  haveTemp2 = false;
  opCode = STORE;
  instruction.addr1 = slowInstruction.addr3;
  buffer = temp1+temp2;
}

void CPU::add(OpCode &opCode, Instruction &instruction, int &buffer) { //need to use currInstruction for addresses that might be lost during intermediate loads/stores -- if error checking, check these instruction vs currInstruction
  //first add
  if(minIndex == -1) {
    passed = false;
    deleteRange(0, LENGTH-1);
    minIndex = LENGTH/2;
    minAddress = currInstruction.addr3;
    insertSum(currInstruction, minIndex);
  } else {
    int sumIndex = calculateIndex(currInstruction.addr3);
    
    //check if before min, if that happens 10 times in a row after the first loop around, reset
    if(currInstruction.addr3 < minAddress) {
      afterMaxCount = 0;
      if(passed) {
        beforeMinCount++;
        if(beforeMinCount > 100) { //tweak 10 to optimize //works if both at 100 but not 10
          //basically starting over
          if(resetRange(opCode, instruction, buffer, 0, LENGTH-1)) {
            minIndex = -1;
            add(opCode, instruction, buffer);
          }
          return;
        } else {
          slowInstruction = currInstruction;
          slowAdd(opCode, instruction, buffer);
        }
        return;
      } else {
        //make sure it's not too low (looped left around back all the way past minIndex), if so slow add
        if(minAddress - currInstruction.addr3 >= LENGTH*4) { //for optimize, consider doing length/2 to keep chunk longer
          slowInstruction = currInstruction;
          slowAdd(opCode, instruction, buffer);
          return;
        } else {
          minIndex = sumIndex;
          minAddress = currInstruction.addr3;
        }
      }
    }
    
    //if we're passing      if we're greater than min but haven't looped around yet-OK otherwise store/reset overlap
    if(currInstruction.addr3 > minAddress) {
      beforeMinCount = 0;
      //make sure it's not too high (looped right around all the way past minIndex), if so slow add
      if(currInstruction.addr3 - minAddress >= LENGTH*4) { //for optimize, consider doing length/2 to keep chunk longer
        afterMaxCount++;
        //if it's happened 10 times in a row, reset
        if(afterMaxCount > 100) { //tweak 10 to optimize
          //basically starting over
          if(resetRange(opCode, instruction, buffer, 0, LENGTH-1)) {
            minIndex = -1;
            add(opCode, instruction, buffer);
          }
          return;
        } else {
          slowInstruction = currInstruction;
          slowAdd(opCode, instruction, buffer);
        }
        return;
      } else {
        //reset overlapping and update min index/address
        if(!resetRange(opCode, instruction, buffer, minIndex, sumIndex)) {
          return;
        }
        deleteRange(minIndex, sumIndex);
        
        if(minIndex != sumIndex) {
          for(int i=1;i<LENGTH;i++) {
            if(sums[(minIndex+i) % LENGTH]) {
              minIndex = (minIndex+i) % LENGTH;
              minAddress += i * 4;
              break;
            }
          }
        }
        passed = true;
      }
    }
    
    //handle case if overwriting the same address
    if(sums[sumIndex] != 0) {
      beforeMinCount = 0;
      afterMaxCount = 0;
      //TODO: check if anything actually changes, return if not
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
      insertSum(currInstruction, sumIndex);
    } else {
      insertSum(currInstruction, sumIndex);
    }
  }
  
  opCode = ADD;
}

//store (along with whatever depends on it) and then set to null for indicies in range, true if finished, false if not
bool CPU::resetRange(OpCode &opCode, Instruction &instruction, int &buffer, int start, int end) { //start at start and keep going until we get to and delete end (so loop around if need be)
  //cover start to end-1 inclusive
  while(start != end) {
    if(sums[start] == 0) {
      start = (start+1) % (LENGTH);
      continue;
    }
    if(sums[start]->inRAM()) {
      for(int i=0;i<LENGTH;i++) {
        if(sums[i]) {
          if(sums[i]->dependsOn(start) && i != start) {
            storeToRAM(opCode, instruction, buffer, i);
            return false;
          }
          //can't delete yet because might be used in those stores --can optimize around here?
          //delete sums[start];
          //sums[start] = 0;
        }
      }
    } else {
      storeToRAM(opCode, instruction, buffer, start); //could be optimized as after store, could delete right away no need to reloop into the if to do so next run
      return false;
    }
    start = (start+1) % (LENGTH);
  }
  //then do end
  if(sums[start]) {
    if(sums[start]->inRAM()) {
      for(int i=0;i<LENGTH;i++) {
        if(sums[i]) {
          if(sums[i]->dependsOn(start) && i != start) {
            storeToRAM(opCode, instruction, buffer, i);
            return false;
          }
          //can't delete yet because might be used in those stores --can optimize around here?
          //delete sums[start];
          //sums[start] = 0;
        }
      }
    } else {
      storeToRAM(opCode, instruction, buffer, start); //could be optimized as after store, could delete right away no need to reloop into the if to do so next run
      return false;
    }
  }
  
  return true;
}

void CPU::deleteRange(int start, int end) {
  //cover start to end-1 inclusive
  while(start != end) {
    if(sums[start] != 0) {
      delete sums[start];
      sums[start] = 0;
    }
    start = (start+1) % (LENGTH);
  }
  //end
  if(sums[start] != 0) {
    delete sums[start];
    sums[start] = 0;
  }
}

unsigned CPU::calculateAddr(int index) {
  int diff = (index - minIndex) % LENGTH;
  if(diff >= 0) {
    return minAddress + diff*4;
  } else {
    return minAddress + (diff + LENGTH)*4;
  }
}

int CPU::calculateIndex(unsigned address) {
  long temp1 = address; //because adding unsigned and signed numbers will convert to unsigned and break -1 mod LENGTH
  long temp2 = minAddress;
  int diff = (minIndex - (temp2 - temp1)/4) % LENGTH;
  if(diff >= 0) {
    return diff;
  } else {
    return diff + LENGTH; //40min, 80 -> 30
  }
}

void CPU::insertValue(unsigned address, int value) {
  sums[calculateIndex(address)] = new Value(value);
}

void CPU::insertSum(Instruction &instruction, int sumIndex) {
  int index1 = calculateIndex(instruction.addr1);
  int index2 = calculateIndex(instruction.addr2);
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
      //std::cout << i << " " << ((Sum*)sums[i])->index1 << " " << ((Sum*)sums[i])->index2 << std::endl;
      storeToRAM(opCode, instruction, buffer, i);
      return;
    }
  }
  
  //load index1 and index2 if not already there
  int index1 = ((Sum*)sums[index])->index1;
  int index2 = ((Sum*)sums[index])->index2;
  //If they're out of range -- this section can be optimized by saving one of the addresses' value if that one is in range
  if((passed && calculateAddr(index1) < minAddress) || (passed && calculateAddr(index2) < minAddress)) {
    slowInstruction.addr1 = calculateAddr(index1);
    slowInstruction.addr2 = calculateAddr(index2);
    slowInstruction.addr3 = calculateAddr(index);
    delete sums[index];
    sums[index] = 0;
    //std::cout << "sloww" << std::endl;
    slowAdd(opCode, instruction, buffer);
    return;
  }
  //load index1 and index2 if not already there and in range
  if(sums[index1] == 0) {
    loadFromRAM(opCode, instruction, index1);
    return;
  }
  if(sums[index2] == 0) {
    loadFromRAM(opCode, instruction, index2);
    return;
  }
  
  //done, so store this
  opCode = STORE;
  instruction.addr1 = calculateAddr(index);
  int val1 = ((Value*)sums[index1])->val;
  int val2 = ((Value*)sums[index2])->val;
  buffer = val1+val2;
  
  //Update
  delete sums[index];
  sums[index] = new Value(buffer);
  //std::cout << "hii" << std::endl;
}