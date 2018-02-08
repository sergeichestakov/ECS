#include "cpu.h"

CPU::CPU() {
}

Instruction currentInstr;
int value1 = 0;
int value2 = 0;

void CPU::operation(OpCode &opCode, Instruction &instruction, int &buffer) {
  switch (opCode) {
    //*addr3 = *addr1 + *addr2
    case ADD:
      currentInstr = instruction;
      loadFromRAM(opCode, instruction, currentInstr.addr1);
      break;
    
    //Buffer contains int requested from RAM
    case LOAD:
      if(instruction.addr1 == currentInstr.addr1) {
        value1 = buffer;
        if(currentInstr.addr1 == currentInstr.addr2) {
          storeToRAM(opCode, instruction, buffer, currentInstr.addr3, value1*2);
          break;
        }
        loadFromRAM(opCode, instruction, currentInstr.addr2);
        break;
      } else {
        value2 = buffer;
        storeToRAM(opCode, instruction, buffer, currentInstr.addr3, value1+value2);
        break;
      }
    
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

void CPU::loadFromRAM(OpCode &opCode, Instruction &instruction, unsigned addr) {
  opCode = LOAD;
  instruction.addr1 = addr;
}

void CPU::storeToRAM(OpCode &opCode, Instruction &instruction, int &buffer, unsigned addr, int value) {
  opCode = STORE;
  instruction.addr1 = addr;
  buffer = value;
}


/*
Notes

start address, then array of indexes for that range?
read in adds until out of 64k ish range (will be lower, need to fine tune)
as you read in, optimize to reduce loads needed so idea= 4=3,2 and 3=1,2 - no need to reget what's in 3 cause it's known - is there an even better way
  don't loop o(n) but start at addr directly casue you can calc index
load as needed in calculations, store everything at once
repeat

next step, store the first x values past the range so we can try and get more in that previous range MAYBE?
*/