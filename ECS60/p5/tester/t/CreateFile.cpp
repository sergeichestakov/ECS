//* Author: Sean Davis
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <cmath>
using namespace std;
enum {N, NE, E, SE, S, SW, W, NW, ALL} ;

class City
{
public:
  int num;
  int roads[8];
  City()
  {
    for(int i = 0; i < 8; i++)
      roads[i] = 0;
  } // City()
}; // class City

struct Pair
{
  int NS, EW;
};

City** createCities(int size)
{
  City **cities;
  int numCities = size * size;
  int *nums = new int[numCities], pos;

  for(int i = 0; i < numCities; i++)
    nums[i] = i;

  cities = new City*[size];

  for(int i = 0; i < size; i++)
  {
    cities[i] = new City[size];

    for(int j = 0; j < size; j++)
    {
      pos = rand() % numCities;
      cities[i][j].num = nums[pos];
      nums[pos] = nums[--numCities];
    }  // for j
  } // for i

  delete [] nums;
  return cities;
} // createCities()


void createRoads(City **cities, int size)
{
  int distance;

  for(int i = 0; i < size - 1; i++)
  {
   for(int j = 0; j < size; j++)
    {
      cities[i][j].roads[E] = cities[i + 1][j].roads[W] = 0 + 3;
      cities[j][i].roads[S] = cities[j][i + 1].roads[N] = 0 + 5;

    }  // for j
  } // for i

  for(int WE = 0; WE < size; WE++)
  {
    for(int NS = 0; NS < size; NS++)
    {
      if(WE < size - 1 && NS < size - 1)
      {
        distance = (int) sqrt(cities[WE][NS].roads[S] * cities[WE + 1][NS].roads[S]
          + cities[WE][NS].roads[E] * cities[WE][NS + 1].roads[E])
          + 0 - 2;
        cities[WE][NS].roads[SE] = cities[WE + 1][NS + 1].roads[NW] = distance;
      }  // if SE possible

      if(WE < size - 1 && NS > 0)
      {
        distance = (int) sqrt(cities[WE][NS].roads[N] * cities[WE + 1][NS].roads[N]
          + cities[WE][NS].roads[S] * cities[WE][NS - 1].roads[S]) + 0 - 2;
        cities[WE][NS].roads[NE] = cities[WE + 1][NS - 1].roads[SW] = distance;
      } // if SW possible

    }  // for NS
  } // for EW
}  // createRoads()


void writeRoads(City **cities, int size, ofstream &outf)
{
  int num2, numCities = size * size;
  Pair *pairs = new Pair[numCities];

  for(int i = 0; i < size; i++)
    for(int j = 0; j < size; j++)
    {
      pairs[i * size + j].NS = i;
      pairs[i * size + j].EW = j;
    } // for j
int lowest = 100000;
  while(numCities > 0)
  {
    int pos = rand() % numCities;
    int EW = pairs[pos].EW;
    int NS = pairs[pos].NS;
    pairs[pos] = pairs[--numCities];
    
    for(int i = 0; i < 8; i++)
      if(cities[EW][NS].roads[i]  > 0)
      {
        

        switch (i)
        {
          case N : num2 = cities[EW][NS - 1].num; break;
          case NE: num2 = cities[EW + 1][NS - 1].num; break;
          case E:  num2 = cities[EW + 1][NS].num; break;
          case SE: num2 = cities[EW + 1][NS + 1].num; break;
          case S:  num2 = cities[EW][NS + 1].num; break;
          case SW: num2 = cities[EW - 1][NS + 1].num; break;
          case W:  num2 = cities[EW - 1][NS].num; break;
          case NW: num2 = cities[EW - 1][NS - 1].num; break;
        } // switch

        if (lowest > cities[EW][NS].roads[i]) {
          std::cout << cities[EW][NS].roads[i];
          lowest = cities[EW][NS].roads[i];
        }

        if(cities[EW][NS].num < num2)
          outf << cities[EW][NS].num << ' ' << num2 << ' '
            << cities[EW][NS].roads[i] << endl;
      } // if a road
  }  // while more cities
  delete [] pairs;
  outf.close();
}  // writeRoads()


void writeTrips(City **cities, int size, ofstream &outf)
{
  int city1, city2, numTrips = size * 5;
  outf << size << ' ' << numTrips << endl;

  for(int i = 0; i < numTrips; i++)
  {
    city1 = rand() % (size * size);

    do
    {
      city2 = rand() % (size * size);
    }  while(city1 == city2);

    outf << city1 << ' '  << city2  << ' ' << 0 << endl;
  } // for i
} // writeTrips()

int main(int argc, char** argv)
{
  int size, seed = 5;
  char filename[80];
  City **cities;

  cout << "Size: ";
  cin >> size;
  cout << "Seed: ";
  cin >> seed;
  srand(seed);
  sprintf(filename, "roads%d-%d.txt", size, seed );
  ofstream outf(filename);
  cities = createCities(size);
  createRoads(cities, size);
  writeTrips(cities, size, outf);
  writeRoads(cities, size, outf);

  for(int i = 0; i < size; i++)
    delete [] cities[i];

  delete [] cities;
  return (EXIT_SUCCESS);
} // main()

