#ifndef OBJECT_H
#define OBJECT_H

#include "point.h"
#include "color.h"
#include <vector>

using namespace std;

class Point {
public:
    Point3f point;
    Color3f color;
    Point3f normal;
    Color3f realColor;
    Point2f raster;
    int id;
public:
    Point():id(0){}
    Point(const Point3f& pp, const Color3f& cc, int idd):point(pp), color(cc), id(idd){}
};

class Triangle {
public:
    Point3i v;
    int id;
    float depth;
public:
    Triangle():id(0), depth(0){}
    Triangle(const Point3i& vv, int idd):v(vv), id(idd), depth(0){}
};

#endif
