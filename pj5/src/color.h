#ifndef COLOR_H
#define COLOR_H

#include <iostream>
#include "base.h"

using namespace std;

class Color3f {
public:
    float r, g, b;
    Color3f():r(0.0), g(0.0), b(0.0){}
    Color3f(float vr, float vg, float vb):r(vr),g(vg), b(vb){}
    Color3f& operator+=(const Color3f& rhs);
    Color3f& operator-=(const Color3f& rhs);
    Color3f& operator*=(const Point3f& rhs);
    Color3f& operator*=(float a);
    Color3f& operator/=(float a);
};

Color3f operator+(const Color3f& l, const Color3f& rhs);
Color3f operator-(const Color3f& l, const Color3f& rhs);
Color3f operator*(const Color3f& l, const Point3f& r);
Color3f operator*(const Color3f& l, float a);
Color3f operator/(const Color3f& l, float a);

istream& operator>>(istream& in, Color3f& p);
ostream& operator<<(ostream& out, const Color3f& p);

#endif
