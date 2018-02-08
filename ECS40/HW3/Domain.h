//
// Domain.h
//

#ifndef DOMAIN_H
#define DOMAIN_H
#include "Shape.h"
#include<vector>
class Domain
{
  public:
    Domain(int sx, int sy);
    void addShape(char type, int x, int y);
    bool fits(const Shape &s) const;
    void draw(void) const;
    bool full(void) const;
  private:
    const int size_x, size_y;
    std::vector<Shape*> sList;
};
#endif
