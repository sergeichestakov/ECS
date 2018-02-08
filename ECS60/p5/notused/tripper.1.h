// Author Sean Davis
#ifndef _TRIPPER_H
#define	_TRIPPER_H

#include "RoadRunner.h"
#include "fibonacciHeap.h"

class Vertex{
  public:
    Vertex() {}
    Vertex(int c, int d) {
      city = c;
      dist = d;
    }
    int city;
    int dist;
    bool operator<(const Vertex &rhs) const {
      return dist < rhs.dist;
    }
    bool operator>(const Vertex &rhs) const {
      return dist > rhs.dist;
    }
};

class Tripper {
public:
  Tripper(Road *roads, int numRoads, int size);
  int findPath(int city1, int city2, int path[], int *pathCount);
  void initializeList(int numRoads, Road * roads);
  void initializeTable();
  void findCity(int city1, int city2, int &dist, int &connection);
  bool dijkstraStep(FibonacciHeap<Vertex> &heap, int knownCol, node<Vertex> * heapLocs[]);
  void generatePath(int connectCity, int city2, int path[], int*pathCount);
  ~Tripper();
private:

}; // class Tripper

#endif	/* _TRIPPER_H */

