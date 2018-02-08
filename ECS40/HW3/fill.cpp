//
// fill.cpp
//

#include "Shape.h"
#include "Domain.h"
#include <cstdlib>

int main(int argc, char **argv)
{
  // use: fill size_x size_y seed

  // domain size
  int size_x = atoi(argv[1]);
  int size_y = atoi(argv[2]);
  // seed for random number generator
  int seed = atoi(argv[3]);
  srand((unsigned) seed);

  // create the domain
  Domain domain(size_x,size_y);

  char type[] = { 'O', 'I', 'L', 'S', 'X', 'U' };

  while ( !domain.full() )
  {
    // try to insert a random shape
    // draw a random number in [0,5]
    // to select a random letter among 'O','I','L','S','X','U'
    int itype = rand() % 6;
    // generate a random position in [0,size_x), [0,size_y)
    int x = rand() % size_x;
    int y = rand() % size_y;
    domain.addShape(type[itype],x,y);
  }

  // draw the domain
  domain.draw();
}
