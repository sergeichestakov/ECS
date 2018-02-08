#ifndef STORAGE_H
#define STORAGE_H

#include "mynew.h"
#include "cpu.h"
#include "QueueAr.h"

struct Address { //15 bytes
  unsigned address;
  short count;
  bool isSum;
  int* value;
  Address* next;
  Address(unsigned addr, short cnt, bool sum, Address* nextInList) {
    address = addr;
    count = cnt;
    isSum = sum;
    next = nextInList;
    value = NULL;
  }
};

class Storage{
  public:
  
  static const int MAXSIZE = 2063; //Prime number
  
  Address** HashTable; //15 bytes per address and a 4 byte pointer so 19 total
  Instruction* instructions; //Instructions are 12 bytes each
  Queue<unsigned int> *addresses;
  int count;
  
  Storage() {
    HashTable = new Address*[MAXSIZE];
    instructions = new Instruction[MAXSIZE];
    addresses = new Queue<unsigned int>(MAXSIZE);
    count = 0;
    for(int i = 0; i < MAXSIZE; i++) {
      HashTable[i] = NULL;
    }
  }
  
  void insert(Instruction instruction){ //Instruction in array, addresses in hash table
    instructions[count] = instruction;
    insertInHashTable(instruction.addr1, count, false);
    insertInHashTable(instruction.addr2, count, false);
    insertInHashTable(instruction.addr3, count, true);
    count++;
  }
  
  //Keep in mind that addresses are inserted in sequential order
  //TODO: not sure what this means? do you not just insert at beginning of list no matter what
  //This means that traversing through the array means going from where the address was last used to where it was first used
  void insertInHashTable(unsigned address, int count, bool sum) {
    int index = hash(address);
    HashTable[index] = new Address(address, count, sum, HashTable[index]);
    if(addresses->isFull()){
      unsigned toDelete = addresses->dequeue();
      deleteAddress(toDelete);
    }
    addresses->enqueue(address);
    
    //After you insert check to see if you can remove an instruction 
    //TODO you only do this optimization once it's full and not before - because it will be covered then anyway - this just adds more time
    if(sum){ //Inserted an address 3
      Address* search = addr->next;
      while(search){
        if(search->address == address){ //Found one with the same address used before it
          if(search->isSum && search->address == instructions[search->count].addr3){ //Delete at that index if it belongs to that set of instructions
            instructions[search->count].addr3 = 0;
          }
          return;
        }
        search = search->next;
      }
    }
  }
  
  //TODO are you not storing things as you delete?
  void deleteAddress(unsigned address){
    int index = hash(address);
    Address* toDelete = HashTable[index];
    Address* prev = NULL;
    while(toDelete && toDelete->address != address){
      prev = toDelete;
      toDelete = toDelete->next;
    }
    //Found it
    if(prev){
      prev->next = toDelete->next;
    } else {
      //TODO it's the head so must update hash table's pointer to the next object or else you lose that whole chain
    }
    delete toDelete;
  }
  
  int* getValue(unsigned address){
    //Find the address in the hash table and then get its value, null if not set
    int index = hash(address);
    Address* search = HashTable[index];
    
    while(search->address != address){
      search = search->next;
    }
    //Found it
    return search->value; //returns NULL if it is not set
  }
  
  void setValue(unsigned address, int* value){ //Sets the value of an address in our hashtable
    int index = hash(address);
    Address* set = HashTable[index];
    while(set->address != address){
      set = set->next;
    } //Found it, so set the value for each address
    while(set){ //Set the same value for each address thats used more than once (different counts)
      if(set->address == address){
        set->value = value;
      }
      set = set->next;
    }
  }
  
  void clear(){
    for(int i = 0; i < MAXSIZE; i++){
      Address* addr = HashTable[i];
      while(addr){
        Address* temp = addr;
        addr = addr->next;
        delete temp;
      }
    }
    delete HashTable;
    delete instructions;
    delete addresses;
  }
  
  void resetCount(){
    count = 0;
  }
  
  int hash(unsigned address){
    return address/4 % MAXSIZE;
  }
  
  bool isFull(){
    return count == MAXSIZE;
  }
  
};

#endif