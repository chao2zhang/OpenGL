#ifndef _GL_H
#define _GL_H

#include <GL/glut.h>
#include <GL/glu.h>
#include <iostream>
#include <cmath>

#define CABINET_INV 0.353553
#define CAVALIER_INV 0.707107

enum ProjType {
    PROJ_XY,
    PROJ_YZ,
    PROJ_XZ
};

class Point2i {
public:
    int x, y;
    Point2i():x(0), y(0){}
    Point2i(int vx, int vy):x(vx), y(vy){}
    int size() const {
        return x * y;
    }
};

class Point2f {
public:
    float x, y;
    Point2f():x(0.0), y(0.0){}
    Point2f(float vx, float vy):x(vx), y(vy){}
    int xInt() const;
    int yInt() const;
};

class Point3f {
public:
    float x, y, z;
    Point3f():x(0.0), y(0.0), z(0.0){}
    Point3f(float vx, float vy, float vz):x(vx), y(vy), z(vz){}
    int xInt() const;
    int yInt() const;
    int zInt() const;
};

class Line {
public:
    float a, b, c;
public:
    Line():a(0), b(0), c(0){}
    Line(float aV, float bV, float cV):a(aV), b(bV), c(cV){}
    Line(int x0, int y0, int x1, int y1);
    Line(const Point2f& l, const Point2f& r);
};

std::istream& operator>>(std::istream& in, Point2f& p);
std::ostream& operator<<(std::ostream& out, const Point2f& p);
std::istream& operator>>(std::istream& in, Point3f& p);
std::ostream& operator<<(std::ostream& out, const Point3f& p);

float area(const Point2f& l, const Point2f& r);
bool intersection(const Line& l, const Line& r, Point2f& i);

std::string format(const std::string &fmt, ...);
void print(void* font, const std::string& s, Point2i& p, const Point2i& window, int h = 13);
#endif
