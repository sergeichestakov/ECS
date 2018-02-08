#ifndef STORE_H
#define STORE_H

#include "mynew.h"
#include "cpu.h"
#include "QueueAr.h"

int currSize = 0;
const int MAXSIZE = 2000;


struct Value{
    int address;
    int value;
    bool sum;
    Value* next;
    Value(int addr, int val){
        address = addr;
        value = val;
        sum = false;
        next = NULL;
    }
};


class Storage {
    public:
    Value** hashTable;
    Queue queue;
    
    Storage(){
        hashTable = new Value*[MAXSIZE];
        queue = new Queue<Value>(MAXSIZE);
        for(int i = 0; i < MAXSIZE; i++){
            hashTable[i] = NULL;
        }
    }
    
    int getValue(int address){ //Returns the value at this address
        int index = hash(address);
        Value* start = hashTable[index];
        while(start->address != address){
            start = start->next;
        }
        return start->value;
    }
    
    void insert(Value newVal){
        //Check if table is full first
        if(currSize == MAXSIZE){
            removeValue();
        }
        
        int index = hash(newVal.address);
        Value* first = hashTable[index];
        if(first == NULL){
            first = newVal;
            return;
        }
        while(first->next){ //While its not null
            first = first->next;
        }
        first->next = newVal;
        queue.enqueue(newVal);
        currSize++;
    }
    
    void removeValue(){
        Value toDelete = queue.dequeue();    
        int index = hash(toDelete.address);
        Value* find = hashTable[index];
        while(find->address != toDelete.address){
            find = find->next;
        }
        
        currSize--;
    }
    
    bool contains(int address){
        int index = hash(address);
        Value* val = hashTable[index];
        while(val){
            if(val->address == address){
                return true;
            }
            val = val->next;
        }
        return false;
    }
    
    int hash(int address){
        return address/4 % MAXSIZE;
    }
    
};

#endif