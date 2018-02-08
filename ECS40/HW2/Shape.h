//
// Shape.h
//

#ifndef SHAPE_H
#define SHAPE_H
class Shape
{
  public:
    virtual ~Shape(void);
    virtual char name(void) const = 0;
    virtual int size(void) const = 0;
    void print(void) const;
    void move (int dx, int dy);
    bool overlap(const Shape &t) const;
    static Shape *makeShape(char ch,int posx,int posy);
  protected:
    int *x, *y;
};

class O: public Shape
{
  public:
    O(int posx, int posy);
    virtual char name(void) const;
    virtual int size(void) const;
};

class I: public Shape
{
  public:
    I(int posx, int posy);
    virtual char name(void) const;
    virtual int size(void) const;
};

class L: public Shape
{
  public:
    L(int posx, int posy);
    virtual char name(void) const;
    virtual int size(void) const;
};

class S: public Shape
{
  public:
    S(int posx, int posy);
    virtual char name(void) const;
    virtual int size(void) const;
};

class X: public Shape
{
  public:
    X(int posx, int posy);
    virtual char name(void) const;
    virtual int size(void) const;
};

class U: public Shape
{
  public:
    U(int posx, int posy);
    virtual char name(void) const;
    virtual int size(void) const;
};
#endif
