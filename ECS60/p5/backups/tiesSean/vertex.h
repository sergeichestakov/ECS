#ifndef _VERTEX_H
#define	_VERTEX_H

class Vertex {
  public:
    Vertex() {}
    Vertex(int c, int d) {
      city = c;
      dist = d;
    }
    int city;
    int dist;
    bool operator<(const Vertex &rhs) const {
      return dist < rhs.dist;
    }
};

#endif