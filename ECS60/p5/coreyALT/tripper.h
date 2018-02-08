// Author Sean Davis
#ifndef _TRIPPER_H
#define	_TRIPPER_H

#include "RoadRunner.h"
#include "VBinaryHeap.h"
#include "vertex.h"

class Tripper {
public:
  Tripper(Road *roads, int numRoads, int size);
  void preproc();
  void resetTempTable();
  int fillLandmark(int city, int index);
  void setHeur(int city1, int city2);
  void setLandmarks(int city1, int city2);
  int findPath(int city1, int city2, int path[], int *pathCount);
  void initializeList(int numRoads, Road * roads);
  void initializeTable();
  void findCity(int city1, int city2, int &dist, int &connection);
  bool dijkstraStep(VBinaryHeap &heap, int &knownCol, int &dir);
  void generatePath(int connectCity, int city2, int path[], int*pathCount);
  ~Tripper();
private:

}; // class Tripper

#endif	/* _TRIPPER_H */

