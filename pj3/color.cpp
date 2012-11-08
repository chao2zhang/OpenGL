#include "color.h"

Color3f& Color3f::operator+=(const Color3f& rhs) {
    r += rhs.r;
    g += rhs.g;
    b += rhs.b;
    return *this;
}

Color3f& Color3f::operator-=(const Color3f& rhs) {
    r -= rhs.r;
    g -= rhs.g;
    b -= rhs.b;
    return *this;
}

Color3f& Color3f::operator*=(float a) {
    r *= a;
    g *= a;
    b *= a;
    return *this;
}

Color3f& Color3f::operator/=(float a) {
    r /= a;
    g /= a;
    b /= a;
    return *this;
}

Color3f operator+(const Color3f& l, const Color3f& rhs) {
    Color3f ret = l;
    ret += rhs;
    return ret;
}

Color3f operator-(const Color3f& l, const Color3f& rhs) {
    Color3f ret = l;
    ret -= rhs;
    return ret;
}

Color3f operator*(const Color3f& l, float a) {
    Color3f ret = l;
    ret *= a;
    return ret;
}

Color3f operator/(const Color3f& l, float a){
    Color3f ret = l;
    ret /= a;
    return ret;
}


istream& operator>>(istream& in, Color3f& p) {
    in >> p.r >> p.g >> p.b;
    return in;
}
ostream& operator<<(ostream& out, const Color3f& p) {
    out << p.r << ' ' << p.g << ' ' << p.b;
    return out;
}
