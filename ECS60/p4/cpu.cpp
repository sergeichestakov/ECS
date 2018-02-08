#include "cpu.h"
#include "storage.h"

CPU::CPU() {
}

Storage *storage = new Storage(100); //do I need to use new both here and inside Storage class/file?
int count = 0;

void CPU::operation(OpCode &opCode, Instruction &instruction, int &buffer) {
  switch (opCode) {
    //*addr3 = *addr1 + *addr2
    case ADD:
      count++;
      storage->track(opCode, instruction, buffer);
      break;
    
    //Buffer contains int requested from RAM
    case LOAD:
      //TODO
      break;
    
    //Sent by RAM after a STORE
    case STORE:
      //TODO
      break;
    
    //All ADDs in file have been sent.  Time to STORE dirty cache.
    case DONE:
      //TODO
      break;
  }
}
