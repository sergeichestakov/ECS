#include "cpu.h"
#include "storage.h"

CPU::CPU() {
}

Storage storage(50000);
Instruction currentInstr;
bool haveVal1 = false;
bool haveVal2 = false;
bool purging = false;
bool readyNext = false;
int value1;
int value2;

int count = 0;

void CPU::operation(OpCode &opCode, Instruction &instruction, int &buffer) {
  switch (opCode) {
    //*addr3 = *addr1 + *addr2
    case ADD:
      count++;
      currentInstr = instruction;
      add(opCode, instruction, buffer);
      break;
    
    //Buffer contains int requested from RAM
    case LOAD:
      //If just loaded a required val, set it locally
      if(!haveVal1) {
        value1 = buffer;
        haveVal1 = true;
      } else if(!haveVal2) {
        value2 = buffer;
        haveVal2 = true;
      }
      
      //Insert into storage
      storage.insert(opCode, instruction, buffer, instruction.addr1, buffer, true);
      
      //Store if a delete was required
      if(opCode == STORE) {
        return;
      }
      
      //Otherwise, back to add process
      add(opCode, instruction, buffer);
      break;
    
    //Sent by RAM after a STORE
    case STORE:
      //If we just finished an add that required a store, get next add
      if(readyNext) {
        readyNext = false;
        opCode = ADD;
        return;
      }
      
      //If we are done with adding and in purging process
      if(purging) {
        storage.purge(opCode, instruction, buffer);
        return;
      }
      
      //Otherwise, resume add process
      add(opCode, instruction, buffer);
      break;
    
    //All ADDs in file have been sent.  Time to STORE dirty cache.
    case DONE:
      purging = true;
      storage.purge(opCode, instruction, buffer);
      break;
  }
}

void CPU::add(OpCode &opCode, Instruction &instruction, int &buffer) {
  //Put 1 and 2 in hashtable if not already there, otherwise get them
  if(!haveVal1 && !storage.get(currentInstr.addr1, value1)) {
    opCode = LOAD;
    instruction.addr1 = currentInstr.addr1;
    return;
  }
  haveVal1 = true;
  if(!haveVal2 && !storage.get(currentInstr.addr2, value2)) {
    opCode = LOAD;
    instruction.addr1 = currentInstr.addr2;
    return;
  }
  haveVal2 = true;
  
  //Check if addr3 is present and overwrite if so, otherwise insert
  storage.update(opCode, instruction, buffer, currentInstr.addr3, value1+value2);
  storage.updateQueue(currentInstr);
  haveVal1 = false;
  haveVal2 = false;
  
  //Store if a delete was required
  if(opCode == STORE) {
    readyNext = true;
    return;
  }
  opCode = ADD;
}