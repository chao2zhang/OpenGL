#ifndef LINE_H
#define LINE_H

#include "point.h"

class Line {
public:
    float a, b, c;
public:
    Line():a(0), b(0), c(0){}
    Line(float aV, float bV, float cV):a(aV), b(bV), c(cV){}
    Line(int x0, int y0, int x1, int y1);
    Line(const Point2f& l, const Point2f& r);
};

#endif
