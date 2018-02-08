#include "tripper.h"
#include "vertex.h"
#include "RoadRunner.h"


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
    table[i] = new int[6]; //City is same as index so it can be 3
  }
  adjList = new Road*[tableSize];
  empty.distance = -1;
  initializeList(numRoads, roads);
} // Tripper()

Tripper::~Tripper() {
  for(int i = 0; i < tableSize; i++){
    delete table[i];
    delete adjList[i];
  }
  delete table;
  delete adjList;
} // ~Tripper()

//Table includes city (same as index), pv, dv, and known
//Heap contains a vertex (city and dist)
int Tripper::findPath(int city1, int city2, int path[], int *pathCount) { //Path is road ID, pathcount is number of edges
  int distance;
  int connectCity;
  findCity(city1, city2, distance, connectCity);
  
  generatePath(connectCity, city2, path, pathCount);
  
  return distance;
}  // findPath()

void Tripper::generatePath(int connectCity, int city2, int path[], int *pathCount) {
  //Up to and including the connecting city
  int currCity = connectCity;
  int backPath[tableSize];
  int length = 0;
  while(currCity != -1) {
    backPath[length] = currCity;
    currCity = table[currCity][0]; //pv
    length++;
  }
  length--;
  
  for(int i=0;i<length;i++) {
    int loc = 0;
    while(adjList[ backPath[length-i] ][loc].city2 != backPath[length-i-1]) {
      loc++;
    }
    path[i] = adjList[ backPath[length-i] ][loc].num;
  }
  
  //The second half
  currCity = connectCity;
  while(currCity != city2) {
    int loc = 0;
    while(adjList[currCity][loc].city2 != table[currCity][3]) {
      loc++;
    }
    path[length] = adjList[currCity][loc].num;
    currCity = table[currCity][3]; //pv of 2nd half
    length++;
  }
  *pathCount = length;
}

void Tripper::findCity(int city1, int city2, int &dist, int &connection) {
  BinaryHeap<Vertex> heap1(tableSize*2, 2);
  heap1.table = table;
  BinaryHeap<Vertex> heap2(tableSize*2, 5);
  heap2.table = table;
  initializeTable();
  
  table[city1][1] = 0;
  Vertex start(city1, 0);
  heap1.insert(start);
  
  table[city2][1] = 0;
  Vertex start2(city2, 0);
  heap2.insert(start2);
  
  //Run and alternate dijkstra steps until they meet
  while(!dijkstraStep(heap1, 0)) {
    if(dijkstraStep(heap2, 3)) {
      break;
    }
  }
  
  int totalDist = 0x7FFFFFFF;
  int connectCity;
  
  //Now that they've met, get the actual shortest path
  for(int i=0;i<tableSize;i++) {
    if((table[i][2] == 1 || table[i][5] == 1) && (table[i][1] < 0x7FFFFFFF && table[i][4] < 0x7FFFFFFF)) {
      int currDist = table[i][1] + table[i][4];
      if(currDist < totalDist) {
        totalDist = currDist;
        connectCity = i;
      }
    }
  }
  
  dist = totalDist;
  connection = connectCity;
}

bool Tripper::dijkstraStep(BinaryHeap<Vertex> &heap, int colOffset) {
  //Take min off top
  Vertex min;
  heap.deleteMin(min);
  
  //Check if already scanned/known, if so check skip
  if(table[min.city][2+colOffset] == 1) {
    return false;
  }
  
  //Terminate if to be scanned was already scanned in other
  table[min.city][2+colOffset] = 1;
  if(table[min.city][2] == table[min.city][5]) {
    return true;
  }
  
  //Loop through all outgoing roads to adjacent cities
  int i = 0;
  while(i != 8 && adjList[min.city][i].distance != -1) {
    Road currRoad = adjList[min.city][i];
    i++;
    
    //already a known city, skip
    if(table[currRoad.city2][2+colOffset] == 1) {
      continue;
    }
    
    //Update table if shorter path found
    int newDv = min.dist + currRoad.distance;
    if(newDv < table[currRoad.city2][1+colOffset]) { //found better path
      table[currRoad.city2][0+colOffset] = min.city;
      table[currRoad.city2][1+colOffset] = newDv;
      Vertex city2(currRoad.city2, newDv);
      heap.insert(city2);
    }
  }
  return false;
}

void Tripper:: initializeTable(){
  for(int i = 0; i < tableSize; i++){ //Initialize table: dv is infinite, pv and known are left as 0
    table[i][0] = -1; //pv1             no previous
    table[i][1] = 0x7FFFFFFF; //dv1     infinite
    table[i][2] = 0; //scanned1         no
    table[i][3] = -1; //pv2             no previous
    table[i][4] = 0x7FFFFFFF; //dv2     infinite
    table[i][5] = 0; //scanned2         no
  }
}

void Tripper::initializeList(int numRoads, Road* roads) {
  for(int i = 0; i < tableSize; i++) {
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