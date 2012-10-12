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
    int size() const {
        return width * height;
    }
};

class Point {
public:
    float x, y;
    Point():x(0.0), y(0.0){}
    Point(float vx, float vy):x(vx), y(vy){}
};

std::istream& operator>>(std::istream& in, Point& p);
std::ostream& operator<<(std::ostream& out, const Point& p);
float area(const Point& l, const Point& r);

extern Color GColor;
extern Size GWindow;

void cleanPixel(GLubyte* pixels);
void makeColor(GLubyte r, GLubyte g, GLubyte b);
void makeColor(const Color& c);
void makePixel(int x, int y, GLubyte* pixels);
void makeLine(int x1, int y1, int x2, int y2, GLubyte* pixels);
void makeLine(const Point& a, const Point& b, GLubyte *pixels);
void makeLineDDA(int x1, int y1, int x2, int y2, GLubyte* pixels);
void makeLineDDA(const Point& a, const Point& b, GLubyte *pixels);

#endif
