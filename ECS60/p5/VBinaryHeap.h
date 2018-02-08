#ifndef _V_BINARY_HEAP_H_
#define _V_BINARY_HEAP_H_

#include "vertex.h"

class VBinaryHeap {
  public:
    int **table;
    explicit VBinaryHeap(int capacity, int col);
    ~VBinaryHeap() { delete array; }

    void insert( const Vertex &x );
    void deleteMin( Vertex &minItem );

  private:
    int tableCol;
    int currentSize;
    int maxSize;
    Vertex *array;

};

#endif
