#ifndef _GL_H
#define _GL_H

#include <GL/glut.h>
#include <iostream>
#include <cmath>

class Color{
public:
    GLubyte r, g, b;
    Color():r(0), g(0), b(0){}
    Color(GLubyte red, GLubyte green, GLubyte blue):r(red), g(green), b(blue){}
};

class Size {
public:
    int width, height;
public:
    Size():width(512), height(512){}
    Size(int w, int h):width(w), height(h){}
    int size() const {
        return width * height;
    }
};

class Rect {
public:
    int left, bottom, right, top;
public:
    Rect():left(0), bottom(0), right(0), top(0){}
    Rect(int l, int b, int r, int t):left(l), bottom(b), right(r), top(t){}
    bool inside(int x, int y) const;
};

class Point {
public:
    float x, y;
    Point():x(0.0), y(0.0){}
    Point(float vx, float vy):x(vx), y(vy){}
    int xInt() const;
    int yInt() const;
};

class Line {
public:
    float a, b, c;
public:
    Line():a(0), b(0), c(0){}
    Line(float aV, float bV, float cV):a(aV), b(bV), c(cV){}
    Line(int x0, int y0, int x1, int y1);
    Line(const Point& l, const Point& r);
};

std::istream& operator>>(std::istream& in, Point& p);
std::ostream& operator<<(std::ostream& out, const Point& p);
float area(const Point& l, const Point& r);
bool intersection(const Line& l, const Line& r, Point& i);

extern Color GColor;
extern Size GWindow;
extern Rect GViewport;
extern void (*makeLine)(int x1, int y1, int x2, int y2, GLubyte* pixels);

void clearPixel(GLubyte* pixels, size_t size);
void makeColor(GLubyte r, GLubyte g, GLubyte b);
void makeColor(const Color& c);
void makePixel(int x, int y, GLubyte* pixels);
void makeLineByPoint(const Point& a, const Point& b, GLubyte *pixels);
void makeLineBres(int x1, int y1, int x2, int y2, GLubyte* pixels);
void makeLineDDA(int x1, int y1, int x2, int y2, GLubyte* pixels);
void makeViewport(int left, int bottom, int right, int top);
void projViewport(GLubyte* pixels);

#endif
