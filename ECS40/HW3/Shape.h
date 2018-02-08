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
    virtual const char* color(void) const = 0;
    int getX(int i) const;
    int getY(int i) const;
    void print(void) const;
    void draw(void) const;
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
    virtual const char* color(void) const;
};

class I: public Shape
{
  public:
    I(int posx, int posy);
    virtual char name(void) const;
    virtual int size(void) const;
    virtual const char* color(void) const;
};

class L: public Shape
{
  public:
    L(int posx, int posy);
    virtual char name(void) const;
    virtual int size(void) const;
    virtual const char* color(void) const;
};

class S: public Shape
{
  public:
    S(int posx, int posy);
    virtual char name(void) const;
    virtual int size(void) const;
    virtual const char* color(void) const;
};

class X: public Shape
{
  public:
    X(int posx, int posy);
    virtual char name(void) const;
    virtual int size(void) const;
    virtual const char* color(void) const;
};

class U: public Shape
{
  public:
    U(int posx, int posy);
    virtual char name(void) const;
    virtual int size(void) const;
    virtual const char* color(void) const;
};
#endif
