#include "color.h"
#include "utils.h"

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

string Color3f::toString() const {
    return format("%.3f %.3f %.3f", r, g, b);
}

istream& operator>>(istream& in, Color3f& p) {
    in >> p.r >> p.g >> p.b;
    return in;
}
ostream& operator<<(ostream& out, const Color3f& p) {
    out << p.r << ' ' << p.g << ' ' << p.b;
    return out;
}

Color3f Color3f::gray() const {
    float gray = 0.299*r + 0.587*g + 0.114*b;
    return Color3f(gray, gray, gray);
}
