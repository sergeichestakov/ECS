//
// testShape.cpp
//

#include "Shape.h"
#include <iostream>
#include <stdexcept>
using namespace std;

int main()
{
  Shape *t1, *t2;
  char ch;
  int x,y;

  try
  {
    cin >> ch >> x >> y;
    t1 = Shape::makeShape(ch,x,y);
    t1->print();
    cin >> ch >> x >> y;
    t2 = Shape::makeShape(ch,x,y);
    t2->print();
    t2->move(1,-1);
    t2->print();

    if ( t1->overlap(*t2) )
      cout << "overlap" << endl;
    else
      cout << "no overlap" << endl;

    delete t1;
    delete t2;
  }
  catch ( invalid_argument &exc )
  {
    cout << exc.what() << ": " << ch << " " << x << " " << y << endl;
  }
}
