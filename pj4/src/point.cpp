#include "point.h"
#include <cmath>

int Point2f::xInt() const {
    return round(x);
}

int Point2f::yInt() const {
    return round(y);
}

Point2f& Point2f::operator+=(const Point2f& r) {
    x += r.x;
    y += r.y;
    return *this;
}

Point2f& Point2f::operator-=(const Point2f& r) {
    x -= r.x;
    y -= r.y;
    return *this;
}

Point2f& Point2f::operator*=(float a) {
    x *= a;
    y *= a;
    return *this;
}
Point2f& Point2f::operator/=(float a) {
    x /= a;
    y /= a;
    return *this;
}

Point2f operator+(const Point2f& l, const Point2f& r) {
    Point2f ret = l;
    ret += r;
    return ret;
}

Point2f operator-(const Point2f& l, const Point2f& r) {
    Point2f ret = l;
    ret -= r;
    return ret;
}

Point2f operator*(const Point2f& l, float a) {
    Point2f ret = l;
    ret *= a;
    return ret;
}

Point2f operator/(const Point2f& l, float a){
    Point2f ret = l;
    ret /= a;
    return ret;
}

Point2f operator*(float a, const Point2f& l) {
    Point2f ret = l;
    ret *= a;
    return ret;
}

Point2f operator/(float a, const Point2f& l){
    Point2f ret = l;
    ret /= a;
    return ret;
}
