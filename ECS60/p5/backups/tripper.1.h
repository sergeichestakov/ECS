// Author Sean Davis
#ifndef _TRIPPER_H
#define	_TRIPPER_H

#include "RoadRunner.h"

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
};

class Tripper {
public:
  Tripper(Road *roads, int numRoads, int size);
  int findPath(int city1, int city2, int path[], int *pathCount);
  void initializeList(int numRoads, Road * roads);
  void initializeTable();
  void findCity(int city1, int city2);
  ~Tripper();
private:

}; // class Tripper

#endif	/* _TRIPPER_H */

