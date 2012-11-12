#ifndef COLOR_H
#define COLOR_H

#include <iostream>

using namespace std;

class Color3f {
public:
    float r, g, b;
    Color3f():r(0.0), g(0.0), b(0.0){}
    Color3f(float vr, float vg, float vb):r(vr),g(vg), b(vb){}
    Color3f& operator+=(const Color3f& rhs);
    Color3f& operator-=(const Color3f& rhs);
    Color3f& operator*=(float a);
    Color3f& operator/=(float a);
    Color3f gray() const;
    string toString() const;
};

Color3f operator+(const Color3f& l, const Color3f& rhs);
Color3f operator-(const Color3f& l, const Color3f& rhs);
Color3f operator*(const Color3f& l, float a);
Color3f operator/(const Color3f& l, float a);

istream& operator>>(istream& in, Color3f& p);
ostream& operator<<(ostream& out, const Color3f& p);

#endif
