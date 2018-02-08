#ifndef _VD_BINARY_HEAP_H_
#define _VD_BINARY_HEAP_H_

#include "vertex.h"

class VDBinaryHeap {
  public:
    int **table;
    explicit VDBinaryHeap(int capacity, int col);
    ~VDBinaryHeap() { delete[] array; }

    void insert( const Vertex &x );
    void deleteMin( Vertex &minItem );

  private:
    int tableCol;
    int currentSize;
    int maxSize;
    Vertex *array;
    int locs[10000];
};

#endif
