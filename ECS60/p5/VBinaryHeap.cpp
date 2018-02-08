#include "VBinaryHeap.h"
#include "vertex.h"

int keys[tableSize];

VBinaryHeap::VBinaryHeap( int capacity, int col ) : currentSize(0) {
    maxSize = capacity;
    array = new Vertex[maxSize];
    tableCol = col;
}

void VBinaryHeap::insert( const Vertex &x ) {
    // Initial hole
    int hole = ++currentSize;
    //Corey's magic size reducing sauce
    if((hole & 1) == 0 && table[ array[hole>>1].city ][tableCol] == 1) {
        hole = hole>>1;
        currentSize--;
    }
    
    //Percolate up
    for( ; hole > 1 && x.dist < array[ hole>>1 ].dist; hole >>= 1 )
        array[ hole ] = array[ hole>>1 ];
    
    array[ hole ] = x;
    keys[x.city] = hole;
}

void VBinaryHeap::deleteMin( Vertex &minItem ) {
    minItem = array[ 1 ];
    
    int hole = 1, succ = 2, sz = currentSize;
    while(succ < sz) {
        int k1 = array[succ].dist;
        int k2 = array[succ+1].dist;
        if(k1 > k2) {
            succ++;
            array[hole].dist = k2;
            array[hole].city = array[succ].city;
        } else {
            array[hole].dist = k1;
            array[hole].city = array[succ].city;
        }
        hole = succ;
        succ <<= 1;
    }
    
    int bubble = array[sz].dist;
    int pred = hole >> 1;
    while(array[pred].dist > bubble) {
        array[hole] = array[pred];
        hole = pred;
        pred >>= 1;
    }
    
    array[hole].dist = bubble;
    array[hole].city = array[sz].city;
    
    currentSize = sz-1;
}

void VBinaryHeap::decreaseKey(Vertex v, int newPos) {
    int city = v.city;
    keys[city] = newPos;
}