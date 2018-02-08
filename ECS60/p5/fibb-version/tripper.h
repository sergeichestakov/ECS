// Author Sean Davis
#ifndef _TRIPPER_H
#define	_TRIPPER_H

#include "RoadRunner.h"
#include "fibheap.h"

class VertexNode : public FibHeapNode {
  public:
    VertexNode() : FibHeapNode() {}
    VertexNode(int c, int d) : FibHeapNode() {
      city = c;
      dist = d;
    }
    
    int city;
    int dist;
    
    virtual void operator=(double newKeyVal) {
      VertexNode v;
      v.dist = newKeyVal;
      dist = newKeyVal;
      FHN_Assign(v);
    }
    virtual void operator=(FibHeapNode& RHS) {
      FHN_Assign(RHS);
      dist = ((VertexNode&) RHS).dist;
    }
    virtual int operator==(FibHeapNode& RHS) {
     if (FHN_Cmp(RHS)) return 0;
     return dist == ((VertexNode&) RHS).dist ? 1 : 0;
    }
    virtual int operator<(FibHeapNode& RHS) {
      int x = FHN_Cmp(RHS);
      if (x != 0) {
        return x < 0 ? 1 : 0;
      } else {
        return dist < ((VertexNode&) RHS).dist ? 1 : 0;
      }
    }
    virtual void print() {
      //nope
    }
};

class Tripper {
public:
  Tripper(Road *roads, int numRoads, int size);
  int findPath(int city1, int city2, int path[], int *pathCount);
  void initializeList(int numRoads, Road * roads);
  void initializeTable();
  void findCity(int city1, int city2, int &dist, int &connection);
  bool dijkstraStep(FibHeap &heap, int knownCol, FibHeapNode * heapLocs[]);
  void generatePath(int connectCity, int city2, int path[], int*pathCount);
  ~Tripper();
private:

}; // class Tripper

#endif	/* _TRIPPER_H */

