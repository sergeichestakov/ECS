//* Author: Sean Davis
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include "RoadRunner.h"
#include "CPUTimer.h"
#include "tripper.h"


using namespace std;

void readFile(const char *filename, Trip **trips, int *size, int *numTrips,
  int *numRoads, Road **roads, Road **roads2)
{
  ifstream inf(filename);
  if(!inf)
  {
    cout << "Unable to open "  << filename << endl;
    exit(1);
  }
  inf >> *size >> *numTrips;
  *trips = new Trip[*numTrips];
  *roads = new Road[*size * *size * 8];
  *roads2 = new Road[*size * *size * 8];

  for(int i = 0; i < *numTrips; i++)
    inf >> (*trips)[i].city1 >> (*trips)[i].city2 >> (*trips)[i].distance;

  for(*numRoads = 0; inf >> (*roads)[*numRoads].city1
    >> (*roads)[*numRoads].city2 >> (*roads)[*numRoads].distance; (*numRoads)++)
  {
    (*roads2)[*numRoads + 1].city2 = (*roads)[*numRoads + 1].city2
       = (*roads2)[*numRoads].city1 = (*roads)[*numRoads].city1;
    (*roads2)[*numRoads + 1].city1 = (*roads)[*numRoads + 1].city1
       = (*roads2)[*numRoads].city2 = (*roads)[*numRoads].city2;
    (*roads2)[*numRoads + 1].distance = (*roads)[*numRoads + 1].distance
      =(*roads2)[*numRoads].distance = (*roads)[*numRoads].distance;
    (*roads2)[*numRoads].num = (*roads)[*numRoads].num = *numRoads;
    (*numRoads)++;
    (*roads2)[*numRoads].num = (*roads)[*numRoads].num = *numRoads;
  }  // for each road
}  // readFile

void checkPath(int distance, Road *roads, int numRoads, int path[],
  int pathCount, Trip trips[], int tripNum)
{
  int total = 0;

  for(int i = 0; i < pathCount; i++)
    if(path[i] < 0 || path[i] >= numRoads)
    {
      cout << "Trip#"<< tripNum << " Path #" << i << " = " << path[i] << " is outside road # range.\n";
      return;
    }

  if(roads[path[0]].city1 != trips[tripNum].city1)
  {
    cout << "Trip#"<< tripNum << " Path #0" << " = " << path[0] << " starts at city #"
      << roads[path[0]].city1  << " which is not start city: "
      << trips[tripNum].city1 << endl;
    return;
  }


  if(roads[path[pathCount - 1]].city2 != trips[tripNum].city2)
  {
    cout << "Trip#"<< tripNum << " Path #" << pathCount - 1 <<  " = " << path[pathCount - 1]
      << " ends at city #"
      << roads[path[pathCount - 1]].city1  << " which is not end city: "
      << trips[tripNum].city2 << endl;
    return;
  }

  for(int i = 0; i < pathCount - 1; i++)
  {
    if(roads[path[i]].city2 != roads[path[i + 1]].city1)
    {
      cout << "Trip#"<< tripNum  << " Path #" << i << " ends at city #" << roads[path[i]].city2
        << " but path #" << i + 1 << " starts at city #"
        << roads[path[i + 1]].city1 << endl;
      return;
    } // if end city and start city don't match

    total += roads[path[i]].distance;

  } // for each road on path

  total += roads[path[pathCount - 1]].distance;

  if(total != distance)
  {
    cout <<  "Trip#"<< tripNum  << " Path total = " << total << " does not match return distance: "
      << distance << endl;
    return;
  } // if path total != distance

 if(distance != trips[tripNum].distance)
   cout << "Trip #" << tripNum << " distance reported as " << distance
    << " but should be " << trips[tripNum].distance << endl;
  
}  // checkPath()

void writeSolutions(char *filename, int distances[])
{
  char filename2[80];
  int size, numTrips, city1, city2, distance;

  ifstream inf(filename);
  *(strchr(filename, '.')) = '\0';

  sprintf(filename2, "%sS.txt", filename);
  ofstream outf(filename2);
  inf >> size >> numTrips;
  outf << size << ' ' << numTrips << endl;

  for(int i = 0; i < numTrips; i++)
  {
    inf >> city1 >> city2 >> distance;
    outf << city1 << ' ' << city2 << ' ' << distances[i] << endl;
  }

  while(inf >> city1 >> city2 >> distance)
    outf << city1 << ' ' << city2 << ' ' << distance << endl;

  outf.close();
} // writeSolutions

int main(int argc, char** argv)
{
  Road *roads, *roads2;
  Trip *trips;
  int *path, pathCount, size, numTrips, numRoads, distance, *distances;
  CPUTimer ct;
  readFile(argv[1], &trips, &size, &numTrips, &numRoads, &roads, &roads2);
  path = new int[size * size];
  distances = new int[numTrips];
  ct.reset();
  Tripper *tripper = new Tripper(roads2, numRoads, size);
  delete[] roads2;

  for(int i = 0; i < numTrips; i++)
  {
    distance = tripper->findPath(trips[i].city1, trips[i].city2, path, &pathCount);

    if(argv[2][0] == '1')
      distances[i] = distance;
    else  // not creating a solution file
      checkPath(distance, roads, numRoads, path, pathCount, trips, i);
  } // for i

  cout << "CPU Time: " << ct.cur_CPUTime() << endl;

  //if(argv[2][0] == '1')
  //  writeSolutions(argv[1], distances);

  delete [] roads;
  delete [] trips;
  delete [] path;
  delete tripper;
  return 0;
} // main()

