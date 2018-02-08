#include "cpu.h"
#include "storage.h"
#include <iostream>

CPU::CPU() {
}

Storage storage(1933);
Instruction currentInstr;
bool haveVal1 = false;
bool haveVal2 = false;
bool purging = false;
bool readyNext = false;
int value1;
int value2;


struct ListItem {     //array might be faster (even with all those moves/copies) because of optimization, idk though
  Instruction instr;
  ListItem *prev;
  ListItem *next;
  ListItem(Instruction instr, ListItem *prev, ListItem *next) : instr(instr), prev(prev), next(next) {}
};


ListItem *listHead = 0;
ListItem *listLooper = 0;
short maxListSize = 100;
short currListSize = 0;
short tracker = 0;

int count = 0;
int count2 = 0;

void CPU::operation(OpCode &opCode, Instruction &instruction, int &buffer) {
  switch (opCode) {
    //*addr3 = *addr1 + *addr2
    case ADD:
      count2++;
      track(opCode, instruction, buffer);
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
        if(purging) {
          processPurge(opCode, instruction, buffer);
          return;
        }
        processEnd(opCode, instruction, buffer);
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
      simplifyList(opCode, instruction, buffer);
      break;
  }
}

void CPU::add(OpCode &opCode, Instruction &instruction, int &buffer) {
  //std::cout << currentInstr.addr1 << "  " << currentInstr.addr2 << "  " << currentInstr.addr3 << std::endl;
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
  haveVal1 = false;
  haveVal2 = false;
  
  //Store if a delete was required
  if(opCode == STORE) {
    readyNext = true;
    return;
  }
  
  if(purging) {
    processPurge(opCode, instruction, buffer);
    return;
  }
  processEnd(opCode, instruction, buffer);
}



void CPU::track(OpCode &opCode, Instruction &instruction, int &buffer) {
  //Insert at beginning of list
  listHead = new ListItem(instruction, 0, listHead);
  if(currListSize > 0) { //try to get rid of this if
    listHead->next->prev = listHead;
  }
  currListSize++;
  
  //Simplify list if full
  if(currListSize == maxListSize) {
    simplifyList(opCode, instruction, buffer);
  }
}

void CPU::simplifyList(OpCode &opCode, Instruction &instruction, int &buffer) { //This could probably be sped up
  //Simplify list
  listLooper = listHead;
  while(listLooper->next != 0) {
    //Look ahead to see if can simplify
    /*ListItem *search = listLooper->next;
    while(search != 0) {
      //Check if listLooper overwrites something with no intermediate dependencies, if so delete what would be overwritten
      if(listLooper->instr.addr3 == search->instr.addr1 || listLooper->instr.addr3 == search->instr.addr2) {
        break;
      }
      if(listLooper->instr.addr3 == search->instr.addr3) {
        //Delete search because it gets overwritten
        ListItem *temp = search->next;
        search->prev->next = temp;
        if(temp != 0) {   //could probably find a way to get rid of this?
          temp->prev = search->prev;
        }
        delete search;
        search = temp;
        currListSize--;
        continue;
      }
      
      //Move down one instruction, search next
      search = search->next;
    }*/
    
    //Move down one instruction, done trying to simplify this one
    listLooper = listLooper->next;
  }
  
  //Done simplifying, store ending section of list -- Note: listLooper now points to last item in list
  tracker = 0;
  if(purging) {
    processPurge(opCode, instruction, buffer);
    return;
  }
  processEnd(opCode, instruction, buffer);
}

void CPU::processEnd(OpCode &opCode, Instruction &instruction, int &buffer) {    //Right now, stores bottom 3/4 of elements, maybe look into doing all values out of range
  count++;
  if(tracker == 1) {//3*currListSize/4) {
    currListSize = currListSize - tracker;
    opCode = ADD;
    return;
  }
  
  currentInstr = listLooper->instr;
  //std::cout << currentInstr.addr1 << "  " << currentInstr.addr2 << "  " << currentInstr.addr3 << std::endl;
  listLooper = listLooper->prev;
  delete listLooper->next;
  listLooper->next = 0;
  tracker++;
  add(opCode, instruction, buffer);
}

void CPU::processPurge(OpCode &opCode, Instruction &instruction, int &buffer) {
  count++;
  if(listLooper == 0) {
    storage.purge(opCode, instruction, buffer);
    return;
  }
  currentInstr = listLooper->instr;
  listLooper = listLooper->prev;
  if(listLooper == 0) {
    delete listHead;
    listHead = 0;
  } else {
    delete listLooper->next;
    listLooper->next = 0;
  }
  currListSize--;
  add(opCode, instruction, buffer);
}