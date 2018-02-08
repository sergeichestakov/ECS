#include "cpu.h"
#include "store.h"

CPU::CPU() {
}

Instruction currInstruction;
Storage storage();
bool hasVal1 = false;
bool hasVal2 = false;

void CPU::operation(OpCode &opCode, Instruction &instruction, int &buffer) {
  switch (opCode) {
    //*addr3 = *addr1 + *addr2
    case ADD:  
      //Check if you have address 1 and 2
      currInstruction = instruction;
      add(opCode, instruction);
      break;
      
    
    //Buffer contains int requested from RAM
    case LOAD:
      if(!hasVal1){
        Value val(currentInstruction.addr1, buffer);
        storage.insert(val);
        hasVal1 = true;
      } else if(!hasVal2){
        Value val(currentInstruction.addr2, buffer);
        storage.insert(val);
        hasVal1 = true;
      }
      add(opCode, instruction);
      break;
    //Sent by RAM after a STORE
    case STORE:
      opCode = ADD;
      break;
    
    //All ADDs in file have been sent.  Time to STORE dirty cache.
    case DONE :
      opCode = DONE;
      break;
  }
}

void CPU::add(OpCode &opCode, Instruction &instruction){
      if(!storage.contains(instruction.addr1)){
        opCode = LOAD;
        return;
      }
      if(!storage.contains(instuction.addr2)){
        instruction.addr1 = instruction.addr2;
        opCode = LOAD;
        return;
      }
      //If you have both values, store it in cache
      int value = storage.getValue(Instruction.addr1) + storage.getValue(Instruction.addr2);
      Value newVal(instruction.addr3, value);
      newVal.sum = true;
      storage.insert(newVal);
      opCode = ADD; 
}