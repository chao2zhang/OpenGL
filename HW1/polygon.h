#ifndef POLYGON_H
#define POLYGON_H

#include "gl.h"
#include <vector>

typedef std::vector<Point>::iterator iterPoly;

class Polygon {
private:
    std::vector<Point> list_;
    Point centroid_;
    bool validCentroid;
    void genCentroid();
public:
    Polygon():validCentroid(false){}
    void push(float x, float y);
    void push(const Point& p);
    void draw(GLubyte* pixels) const;
    void translate(float x, float y);
    void translate(const Point& p);
    void rotate(float a);
    void scale(float a);
    const Point& centroid();
    const std::vector<Point>& list() const;
};
#endif
