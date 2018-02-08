#ifndef STORAGE_H
#define STORAGE_H

#include "cpu.h"
#include "mynew.h"

struct ListItem {     //array might be faster (even with all those moves/copies) because of optimization, idk though
  Instruction instr;
  ListItem *prev;
  ListItem *next;
  ListItem(Instruction instr, ListItem *prev, ListItem *next) : instr(instr), prev(prev), next(next) {}
};

class Storage {
  public:
    ListItem *listHead;
    ListItem *listLooper;
    short maxListSize;
    short currListSize;
    unsigned maxAddress;
  
    Storage(short maxListSize) : maxListSize(maxListSize) {
      listHead = 0;
      listLooper = 0;
      currListSize = 0;
      maxAddress = 0; //TODO use but be careful about switching processes
    }
    
    void track(OpCode &opCode, Instruction &instruction, int &buffer) {
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
    
    void simplifyList(OpCode &opCode, Instruction &instruction, int &buffer) { //This could probably be sped up
      //Simplify list
      listLooper = listHead;
      while(listLooper->next != 0) {
        //Look ahead to see if can simplify
        ListItem *search = listLooper->next;
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
        }
        
        //Move down one instruction, done trying to simplify this one
        listLooper = listLooper->next;
      }
      
      //Done simplifying, store ending section of list -- Note: listLooper now points to last item in list
      processEnd(opCode, instruction, buffer);
    }
    
    void processEnd(OpCode &opCode, Instruction &instruction, int &buffer) {    //Right now, stores bottom 3/4 of elements, maybe look into doing all values out of range
      for(int i=0;i<3*currListSize/4;i++) {
        listLooper = listLooper->prev;
        delete listLooper->next;
        listLooper->next = 0;
      }
      currListSize = currListSize - 3*currListSize/4;
    }
};

#endif