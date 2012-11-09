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
    string toString() const;
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
void projectOrtho(Point& p, int option);
void projectOrtho(vector<Point>& p, int option);
void depth(const vector<Point>& p, vector<Triangle>& t, const Point3f& r, const Point3f& v);
void normal(vector<Point>& p, const vector<Triangle>& t, int n);
void sort(const vector<Point>& p, vector<Triangle>& t, const Point3f& ref, const Point3f& view);
void light(Point& p, float ka, float kd, float ks, const Color3f& iA, const Color3f& iL, const Point3f& lum, const Point3f& ref, int sp);
void light(vector<Point>& p, float ka, float kd, float ks, const Color3f& iA, const Color3f& iL, const Point3f& lum, const Point3f& ref, int sp);
template <class T> void randomize(vector<T>& v);
void makePixel(int x, int y, const Color3f& c, GLfloat *pixels, int ppl);
void pixel(int x, int y, const Color3f& c, GLfloat *pixels, int ppl);
void line(const Point& a, const Point& b, GLfloat* pixels, int ppl);
void horizontalLine(int begX, int endX, int y, const Color3f& begC, const Color3f& endC, GLfloat* pixels, int ppl);
void triangle(const Point& a, const Point& b, const Point& c, GLfloat* pixels, int ppl);
void triangle(const vector<Point>& p, const vector<Triangle>& t, GLfloat* pixels, int ppl);
string toString(const vector<Point>& p, const vector<Triangle>& t, int num);
#endif
