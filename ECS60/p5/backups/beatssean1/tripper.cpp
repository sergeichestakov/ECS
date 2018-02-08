#include "tripper.h"
#include "vertex.h"
#include "RoadRunner.h"
#include <cstdlib>


#include <iostream>
using namespace std;

int **table;
int **landmarks;
int *currLands;
int *currHeur;
Road **adjList;
Road empty;
int tableSize;
int totalRoads;
int numLandmarks;
int active;


Tripper::Tripper(Road *roads, int numRoads, int size) {
  numLandmarks = 16;
  active = 1;
  tableSize = size * size;
  totalRoads = numRoads;
  table = new int*[tableSize];
  landmarks = new int*[tableSize];
  currLands = new int[active];
  currHeur = new int[tableSize];
  for(int i = 0; i < tableSize; i++) {
    table[i] = new int[6]; //City is same as index so it can be 3
    landmarks[i] = new int[numLandmarks];
  }
  adjList = new Road*[tableSize];
  empty.distance = -1;
  initializeList(numRoads, roads);
  
  preproc();
} // Tripper()

Tripper::~Tripper() {
  for(int i = 0; i < tableSize; i++){
    delete table[i];
    delete adjList[i];
    delete landmarks[i];
  }
  delete table;
  delete adjList;
  delete landmarks;
  delete currLands;
  delete currHeur;
} // ~Tripper()

void Tripper::preproc() {
  int city;
  for(int i=0;i<numLandmarks;i++) {
    city = rand() % tableSize;
    //cout << city << endl;
    fillLandmark(city, i);
  }
}

void Tripper::resetTempTable() {
  for(int i=0;i<tableSize;i++) {
    table[i][1] = 0x7FFFFFFF;
    table[i][2] = 0;
  }
}

//Run dijkstra using table[1] and [2], place finals into landmarks[city][index]
void Tripper::fillLandmark(int city, int index) {
  BinaryHeap<Vertex> heap(tableSize*2, 2);
  heap.table = table;
  resetTempTable();
  int count = 0;

  table[city][1] = 0;
  Vertex start(city, 0);
  heap.insert(start);
  
  while(count != tableSize) { //City 2 is not known
    Vertex min;
    heap.deleteMin(min);
    if(table[min.city][2] == 1) {
      continue;
    }
    table[min.city][2] = 1;
    landmarks[min.city][index] = table[min.city][1];
    count++;

    int i = 0;
    while(i != 8 && adjList[min.city][i].distance != -1) { //loop through outgoing roads to adjacent cities
      Road currRoad = adjList[min.city][i];
      i++;
      if(table[currRoad.city2][2] == 1) { //already a known city, skip
        continue;
      }
      
      int newDv = table[min.city][1] + currRoad.distance;
      if(newDv < table[currRoad.city2][1]) { //found better path
        table[currRoad.city2][1] = newDv;
        Vertex city2(currRoad.city2, newDv);
        heap.insert(city2);
      }
    }
  }
  
}

void Tripper::setHeur(int city1, int city2) {
  int currLand, cityPath, diff1, diff2, diff;
  unsigned temp;
  for(int i=0;i<tableSize;i++) {
    for(int j=0;j<active;j++) {
      currLand = currLands[j];
      cityPath = landmarks[i][currLand];
      
      diff1 = landmarks[city2][currLand] - cityPath;
      temp = diff1 >> 31;
      diff1 ^= temp;
      diff1 += temp & 1;
      
      diff2 = landmarks[city1][currLand] - cityPath;
      temp = diff2 >> 31;
      diff2 ^= temp;
      diff2 += temp & 1;
      
      diff = (diff1-diff2)/2;

      if(j == 0) {
        currHeur[i] = diff;
      } else if(diff > currHeur[i]) {
        currHeur[i] = diff;
      }
    }
  }
  
}

void Tripper::setLandmarks(int city1, int city2) {
  for(int i=0;i<active;i++) {
    int max = 0;
    for(int j=0;j<numLandmarks;j++) {
      int first = landmarks[city1][j];
      int second = landmarks[city2][j];
      
      //Get absolute difference
      int diff = first - second;
      unsigned temp = diff >> 31;
      diff ^= temp;
      diff += temp & 1;
      
      if(diff > max) {
        bool used = false;
        for(int k=0;k<i;k++) {
          if(currLands[k] == j) {
            used = true;
          }
        }
        if(!used) {
          max = diff;
          currLands[i] = j; //want highest diffs out of all
        }
      }
    }
  }
  
}

//Table includes city (same as index), pv, dv, and known
//Heap contains a vertex (city and dist)
int Tripper::findPath(int city1, int city2, int path[], int *pathCount) { //Path is road ID, pathcount is number of edges
  setLandmarks(city1, city2);
  setHeur(city1, city2);
  
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
    //cout << adjList[ backPath[length-i] ][loc].city2 << " ";
  }
  
  //The second half
  currCity = connectCity;
  while(currCity != city2) {
    int loc = 0;
    while(adjList[currCity][loc].city2 != table[currCity][3]) {
      loc++;
    }
    path[length] = adjList[currCity][loc].num;
    //cout << adjList[currCity][loc].city2 << " ";
    currCity = table[currCity][3]; //pv of 2nd half
    length++;
  }
  //cout << endl;
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
  
  table[city2][4] = 0;
  Vertex start2(city2, 0);
  heap2.insert(start2);
  
  //Run and alternate dijkstra steps until they meet
  while(!dijkstraStep(heap1, 0, 1)) {
    if(dijkstraStep(heap2, 3, -1)) {
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
        //cout << " hey"<<currDist;
        totalDist = currDist;
        connectCity = i;
      }
    }
  }
  //cout << "."<<connectCity<<".";
  
  dist = totalDist;
  connection = connectCity;
}

bool Tripper::dijkstraStep(BinaryHeap<Vertex> &heap, int colOffset, int dir) {
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
    
    int newDv = table[min.city][1+colOffset] + currRoad.distance;// + diff;
    if(newDv < table[currRoad.city2][1+colOffset]) { //found better path
      table[currRoad.city2][0+colOffset] = min.city;
      table[currRoad.city2][1+colOffset] = newDv;
      Vertex city2(currRoad.city2, newDv+dir*currHeur[currRoad.city2]);
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