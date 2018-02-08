#include "tripper.h"
#include "vertex.h"
#include "RoadRunner.h"
#include <cstdlib>


#include <iostream>
using namespace std;

int **table;
int **landmarks;
int *currHeur;
Road **adjList;
Road empty;
int tableSize;
int totalRoads;
int numLandmarks;
int active;


Tripper::Tripper(Road *roads, int numRoads, int size) {
  numLandmarks = 16;
  tableSize = size * size;
  totalRoads = numRoads;
  table = new int*[tableSize];
  landmarks = new int*[tableSize];
  currHeur = new int[tableSize];
  for(int i = 0; i < tableSize; i++) {
    table[i] = new int[6]; //City is same as index so it can be 3
    landmarks[i] = new int[numLandmarks];
  }
  adjList = new Road*[tableSize];
  empty.distance = -1;
  initializeList(numRoads, roads);
  
  int *currRow;
  for(int i = 0; i < tableSize; i++){ //Initialize table: dv is infinite, pv and known are left as 0
    currRow = table[i];
    currRow[0] = -1; //pv1             no previous
    currRow[3] = -1; //pv2             no previous
  }
  
  preproc();
} // Tripper()

Tripper::~Tripper() {
  for(int i = 0; i < tableSize; i++){
    delete[] table[i];
    delete[] adjList[i];
    delete[] landmarks[i];
  }
  delete[] table;
  delete[] adjList;
  delete[] landmarks;
  delete[] currHeur;
} // ~Tripper()

int maxLandDist = 0;
void Tripper::preproc() {
  int lands[numLandmarks];
  int city = rand() % tableSize;
  for(int i=0;i<numLandmarks;i++) {
    //cout << city << endl;
    lands[i] = city;
    city = fillLandmark(city, i);
    for(int j=0;j<=i;j++) {
      if(city == lands[j]) {
        city = rand() % tableSize;
        break;
      }
    }
    maxLandDist = 0;
  }
}

void Tripper::resetTempTable() {
  for(int i=0;i<tableSize;i++) {
    table[i][1] = 0x7FFFFFFF;
    table[i][2] = 0;
  }
}

//Run dijkstra using table[1] and [2], place finals into landmarks[city][index]
int Tripper::fillLandmark(int city, int index) {
  VBinaryHeap heap(tableSize*2, 2);
  heap.table = table;
  resetTempTable();
  int count = 0;

  table[city][1] = 0;
  Vertex start(city, 0);
  heap.insert(start);
  
  int ret;
  
  while(count != tableSize) { //City 2 is not known
    Vertex min;
    heap.deleteMin(min);
    if(table[min.city][2] == 1) {
      continue;
    }
    table[min.city][2] = 1;
    landmarks[min.city][index] = table[min.city][1];
    if(table[min.city][1] > maxLandDist) {
      maxLandDist = table[min.city][1];
      ret = min.city;
    }
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
  
  return ret;
}

void Tripper::setHeur(int city1, int city2) {
  int cityPath, diff1, diff2;
  unsigned temp;
  for(int i=0;i<tableSize;i++) {
    cityPath = landmarks[i][active];
    
    diff1 = landmarks[city2][active] - cityPath;
    temp = diff1 >> 31;
    diff1 ^= temp;
    diff1 += temp & 1;
    
    diff2 = landmarks[city1][active] - cityPath;
    temp = diff2 >> 31;
    diff2 ^= temp;
    diff2 += temp & 1;
    
    currHeur[i] = (diff1-diff2)/2;
  }
  
}

void Tripper::setLandmarks(int city1, int city2) {
  int max = 0;
  for(int i=0;i<numLandmarks;i++) {
    int first = landmarks[city1][i];
    int second = landmarks[city2][i];
    
    //Get absolute difference
    int diff = first - second;
    unsigned temp = diff >> 31;
    diff ^= temp;
    diff += temp & 1;
    
    if(diff > max) {
      active = i;
      max = diff;
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
  VBinaryHeap heap1(tableSize*2, 2);
  heap1.table = table;
  VBinaryHeap heap2(tableSize*2, 5);
  heap2.table = table;
  initializeTable();
  
  table[city1][0] = -1;
  table[city1][1] = 0;
  Vertex start(city1, 0);
  heap1.insert(start);
  
  table[city2][3] = -1;
  table[city2][4] = 0;
  Vertex start2(city2, 0);
  heap2.insert(start2);
  
  //Run and alternate dijkstra steps until they meet
  int col1 = 0, dir1 = 1, col2 = 3, dir2 = -1;
  while(!dijkstraStep(heap1, col1, dir1)) {
    if(dijkstraStep(heap2, col2, dir2)) {
      break;
    }
  }
  
  int totalDist = 0x7FFFFFFF;
  int connectCity;
  
  //Now that they've met, get the actual shortest path
  int *currRow;
  for(int i=0;i<tableSize;i++) {
    currRow = table[i];
    if((currRow[2] == 1 || currRow[5] == 1) && (currRow[1] < 0x7FFFFFFF && currRow[4] < 0x7FFFFFFF)) {
      int currDist = currRow[1] + currRow[4];
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

bool Tripper::dijkstraStep(VBinaryHeap &heap, int &colOffset, int &dir) {
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
  int *currRow;
  for(int i = 0; i < tableSize; i++){ //Initialize table: dv is infinite, pv and known are left as 0
    currRow = table[i];
    currRow[1] = 0x7FFFFFFF; //dv1     infinite
    currRow[2] = 0; //scanned1         no
    currRow[4] = 0x7FFFFFFF; //dv2     infinite
    currRow[5] = 0; //scanned2         no
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