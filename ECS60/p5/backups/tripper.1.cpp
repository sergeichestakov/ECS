#include "tripper.h"
#include "RoadRunner.h"
#include "BinaryHeap.h"

#include <iostream>
using namespace std;

int **table;
Road **adjList;
Road empty;
int tableSize;
int totalRoads;

Tripper::Tripper(Road *roads, int numRoads, int size) {
  tableSize = size * size;
  totalRoads = numRoads;
  table = new int*[tableSize];
  for(int i = 0; i < tableSize; i++) {
    table[i] = new int[3]; //City is same as index so it can be 3
  }
  adjList = new Road*[numRoads];
  empty.distance = -1;
  initializeList(numRoads, roads);
} // Tripper()

Tripper::~Tripper() {
  for(int i = 0; i < tableSize; i++){
    delete [] table[i];
  }
  delete [] table;
  
  for(int i = 0; i < totalRoads; i++){
    delete [] adjList[i];
  }
  delete [] adjList;
} // ~Tripper()

//Table includes city (same as index), pv, dv, and known
//Heap contains a vertex (city and dist)
int Tripper::findPath(int city1, int city2, int path[], int *pathCount) { //Path is road ID, pathcount is number of edges
  findCity(city1, city2);
  
  //Find path and distance
  int currCity = city2;
  int backPath[tableSize];
  int length = 0;
  while(currCity != -1) {
    backPath[length] = currCity;
    currCity = table[currCity][0]; //pv
    length++;
  }
  length--;
  *pathCount = length;
  
  for(int i=0;i<length;i++) {
    int loc = 0;
    while(adjList[ backPath[length-i] ][loc].city2 != backPath[length-i-1]) {
      loc++;
    }
    path[i] = adjList[ backPath[length-i] ][loc].num;
  }
  
  return table[city2][1];
}  // findPath()

void Tripper::findCity(int city1, int city2) {
  BinaryHeap<Vertex> heap(tableSize);
  initializeTable();

  table[city1][1] = 0;
  Vertex start(city1, 0);
  heap.insert(start);
  
  
  while(table[city2][2] != 1) { //City 2 is not known
    Vertex min;
    heap.deleteMin(min);
    if(table[min.city][2] == 1) {
      continue;
    }
    table[min.city][2] = 1;
    
    int i = 0;
    while(i != 8 && adjList[min.city][i].distance != -1) { //loop through outgoing roads to adjacent cities
      Road currRoad = adjList[min.city][i];
      i++;
      if(table[currRoad.city2][2] == 1) { //already a known city, skip
        continue;
      }
      
      int newDv = table[min.city][1] + currRoad.distance;
      if(newDv < table[currRoad.city2][1]) { //found better path
        table[currRoad.city2][0] = min.city;
        table[currRoad.city2][1] = newDv;
        Vertex city2(currRoad.city2, newDv);
        heap.insert(city2);
      }
    }
  }
}

void Tripper:: initializeTable(){
  for(int i = 0; i < tableSize; i++){ //Initialize table: dv is infinite, pv and known are left as 0
    table[i][0] = -1; //pv, unknown
    table[i][1] = 0x7FFFFFFF; //dv, inf
    table[i][2] = 0; //finished, unknown
  }
}

void Tripper::initializeList(int numRoads, Road* roads) {
  for(int i = 0; i < numRoads; i++) {
    adjList[i] = new Road[8];
    adjList[i][0] = empty;
  }
  for(int i = 0; i < numRoads; i++) {
    int city1 = roads[i].city1;
    int j;
    for(j = 0; adjList[city1][j].distance != -1; j++);
    adjList[city1][j] = roads[i];
    if(j + 1 < 8) {
      adjList[city1][j + 1] = empty;
    }
  }
}
