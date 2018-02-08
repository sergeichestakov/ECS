#include "cpu.h"
#include "storage.h"
//#include <iostream>

CPU::CPU() {
}

Storage storage;
int currIndex = 0;
int* val1 = NULL;
int* val2 = NULL;
Instruction currInst;
Instruction* optimized;

void CPU::operation(OpCode &opCode, Instruction &instruction, int &buffer) {
  switch (opCode) {
    //*addr3 = *addr1 + *addr2
    case ADD:
      if(!storage.isFull()){
        storage.insert(instruction);
        break;
      }
      storage.resetCount();
      //Loop through the array and load every single addr1 and addr2 and store in addr3
      optimized = storage.instructions;
      process(optimized, opCode, instruction, buffer);
      break;
    
    //Buffer contains int requested from RAM
    case LOAD:
      if(!val1){ //Load requested the value at address1
        val1 = &buffer;
        storage.setValue(currInst.addr1, val1);
      } else if(!val2){ //Load wanted address2
        val2 = &buffer;
        storage.setValue(currInst.addr2, val2);
      }
      process(optimized, opCode, instruction, buffer); //Continue to process
      break;
    
    //Sent by RAM after a STORE
    case STORE:
      process(optimized, opCode, instruction, buffer); //Keep processing next instruction
      break;
    
    //All ADDs in file have been sent.  Time to STORE dirty cache.
    case DONE:
      //Load in the remaining values from RAM and store them
      optimized = storage.instructions;
      currIndex = 0;
      for(int i = 0; i < storage.count; i++){
        process(optimized, opCode, instruction, buffer);  
      }
      storage.clear();
      opCode = DONE;
  }
}

void CPU::process(Instruction* optimized, OpCode &opCode, Instruction &instruction, int &buffer){
  if(currIndex == storage.MAXSIZE){ //You have traversed the entire array, so clear the arrays and request add
    currIndex = 0;
    opCode = ADD; //Request the next set of adds
    return;
  }
  //First check to make sure that this is not a deleted value
  currInst = optimized[currIndex];
  if(currInst.addr3 == 0){ //Empty, so skip over it
    currIndex++;
    return;
  }
  //Then see if you have the values at the addresses already stored
  if(!val1){//Check to see if its in storage first
    val1 = storage.getValue(currInst.addr1);
  }
  if(!val1){//Not in storage, so load from RAM
    instruction.addr1 = currInst.addr1;
    opCode = LOAD;
    return;
  }
  if(!val2){
    val2 = storage.getValue(currInst.addr2);
  }
  if(!val2){
    instruction.addr1 = currInst.addr2;
    opCode = LOAD;
    return;
  }
  //You have both values, so sum them
  instruction.addr1 = currInst.addr3;
  int sum = *val1 + *val2;
  buffer = sum;
  int* sumPtr = &sum;
  storage.setValue(currInst.addr3, sumPtr); //Update the value in storage
  opCode = STORE;
  val1 = NULL; //Reset pointers and move on to next index
  val2 = NULL;
  currIndex++;
}
