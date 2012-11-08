#include "point.h"

using namespace std;

istream& operator>>(istream& in, Point2f& p) {
    in >> p.x >> p.y;
    return in;
}

istream& operator>>(istream& in, Point3f& p) {
    in >> p.x >> p.y >> p.z;
    return in;
}

istream& operator>>(istream& in, Point3i& p) {
    in >> p.x >> p.y >> p.z;
    return in;
}

ostream& operator<<(ostream& out, const Point2f& p) {
    out << p.x << ' ' << p.y;
    return out;
}

ostream& operator<<(ostream& out, const Point3f& p) {
    out << p.x << ' ' << p.y << ' ' << p.z;
    return out;
}

ostream& operator<<(ostream& out, const Point3i& p) {
    out << p.x << ' ' << p.y << ' ' << p.z;
    return out;
}

int Point2f::xInt() const {
    return round(x);
}

int Point2f::yInt() const {
    return round(y);
}

int Point3f::xInt() const {
    return round(x);
}

int Point3f::yInt() const {
    return round(y);
}

int Point3f::zInt() const {
    return round(z);
}

float Point3f::length() const {
    return sqrt(x*x + y*y + z*z);
}

Point3f& Point3f::operator+=(const Point3f& r) {
    x += r.x;
    y += r.y;
    z += r.z;
    return *this;
}

Point3f& Point3f::operator-=(const Point3f& r) {
    x -= r.x;
    y -= r.y;
    z -= r.z;
    return *this;
}

Point3f& Point3f::operator*=(float a) {
    x *= a;
    y *= a;
    z *= a;
    return *this;
}
Point3f& Point3f::operator/=(float a) {
    x /= a;
    y /= a;
    z /= a;
    return *this;
}

Point3f operator+(const Point3f& l, const Point3f& r) {
    Point3f ret = l;
    ret += r;
    return ret;
}

Point3f operator-(const Point3f& l, const Point3f& r) {
    Point3f ret = l;
    ret -= r;
    return ret;
}

Point3f operator*(const Point3f& l, float a) {
    Point3f ret = l;
    ret *= a;
    return ret;
}

Point3f operator/(const Point3f& l, float a){
    Point3f ret = l;
    ret /= a;
    return ret;
}

Point3f operator*(float a, const Point3f& l) {
    Point3f ret = l;
    ret *= a;
    return ret;
}

Point3f operator/(float a, const Point3f& l){
    Point3f ret = l;
    ret /= a;
    return ret;
}
