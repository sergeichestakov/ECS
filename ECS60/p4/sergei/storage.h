#ifndef STORAGE_H
#define STORAGE_H

#include "mynew.h"
#include "cpu.h"

struct Value {
  unsigned address;
  int value;
  bool stored;
  Value *next;
  Value(unsigned nAddress, int nValue, bool nStored, Value *nNext) {
    address = nAddress;
    value = nValue;
    stored = nStored;
    next = nNext;
  }
};

class Storage {
  public:
      Value **hashStore;
      unsigned *addressQueue;
      short maxSize;
      short currSize;
      short queueFront;
      short queueBack;
      
      Storage(short size) {
        maxSize = size;
        currSize = 0;
        queueFront = 0;
        queueBack = 0;
        hashStore = new Value*[maxSize];
        for(short i=0;i<size;i++) {
          hashStore[i] = 0;
        }
        addressQueue = new unsigned[maxSize];
      }
      
      void insert(OpCode &opCode, Instruction &instruction, int &buffer, unsigned address, int value, bool stored) {
        //Delete/store if there is not enough space
        if(currSize == maxSize) {
          //Get the address to be deleted
          unsigned toDelete = addressQueue[queueFront];
          queueFront = (queueFront+1) % maxSize;
          
          //Find the Value to be deleted
          if(hashStore[toDelete/4 % maxSize]->next == 0) {
            store(opCode, instruction, buffer, hashStore[toDelete/4 % maxSize]);
            delete hashStore[toDelete/4 % maxSize];
            hashStore[toDelete/4 % maxSize] = 0;
          } else {
            Value *start = hashStore[toDelete/4 % maxSize];
            while(start->next->next) {
              start = start->next;
            }
            store(opCode, instruction, buffer, start->next);
            delete start->next;
            start->next = 0;
          }
          
          //Done deleting
          currSize--;
        }
        
        //Insert new value at beginning, point next to what used to be beginning (null if it was empty)
        hashStore[address/4 % maxSize] = new Value(address, value, stored, hashStore[address/4 % maxSize]);
        
        //Insert address into queue
        addressQueue[queueBack] = address;
        queueBack = (queueBack+1) % maxSize;
        
        //Done
        currSize++;
      }
      
      bool store(OpCode &opCode, Instruction &instruction, int &buffer, Value *value) {
        if(value->stored == false) {
          opCode = STORE;
          instruction.addr1 = value->address;
          buffer = value->value;
          return true;
        }
        return false;
      }
      
      bool get(unsigned address, int &set) {
        //Starting pointer
        Value *start = hashStore[address/4 % maxSize];
        
        //Loop until null (end)
        while(start) {
          if(start->address == address) {
            set = start->value;
            return true;
          }
          start = start->next;
        }
        
        //Wasn't found
        return false;
      }
      
      void update(OpCode &opCode, Instruction &instruction, int &buffer, unsigned address, int value) {
        //Starting pointer
        Value *start = hashStore[address/4 % maxSize];
        
        //Loop until null (end)
        while(start) {
          if(start->address == address) {
            start->value = value;
            start->stored  = false;
            return;
          }
          start = start->next;
        }
        
        //Wasn't found, so insert
        insert(opCode, instruction, buffer, address, value, false);
      }
      
      void purge(OpCode &opCode, Instruction &instruction, int &buffer) {
        while(currSize != 0) {
          //Get the address to be deleted
          unsigned toDelete = addressQueue[queueFront];
          queueFront = (queueFront+1) % maxSize;
          
          //Store it if needed, then delete
          Value *next = hashStore[toDelete/4 % maxSize]->next;
          if(store(opCode, instruction, buffer, hashStore[toDelete/4 % maxSize])) {
            delete hashStore[toDelete/4 % maxSize];
            hashStore[toDelete/4 % maxSize] = next;
            currSize--;
            return;
          }
          //No store was needed, so continue
          delete hashStore[toDelete % maxSize];
          hashStore[toDelete/4 % maxSize] = next;
          currSize--;
          
        }
        //Empty so send DONE opCode
        opCode = DONE;
        return;
      }
};

#endif