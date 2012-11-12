//#define TEST 1

#ifndef POINT_H
#define POINT_H

#include <GL/glut.h>
#include <cmath>
#include <iostream>

#define PI 3.141593
#define CABINET_INV 0.353553
#define CAVALIER_INV 0.707107

#define AXIS_X 0
#define AXIS_Y 1
#define AXIS_Z 2
#define PLANE_XY 10
#define PLANE_YZ 11
#define PLANE_XZ 12
#define PLANE_YX 13
#define PLANE_ZX 14
#define PLANE_ZY 15

class Point2i {
public:
    int x, y;
    Point2i():x(0), y(0){}
    Point2i(int vx, int vy):x(vx), y(vy){}
};

class Point2f {
public:
    float x, y;
    Point2f():x(0.0), y(0.0){}
    Point2f(float vx, float vy):x(vx), y(vy){}
    int xInt() const;
    int yInt() const;
};


class Point3i {
public:
    int x, y, z;
    Point3i():x(0), y(0), z(0){}
    Point3i(int vx, int vy, int vz):x(vx), y(vy), z(vz){}
};

class Point3f {
public:
    float x, y, z;
    Point3f():x(0.0), y(0.0), z(0.0){}
    Point3f(float vx, float vy, float vz):x(vx), y(vy), z(vz){}
    int xInt() const;
    int yInt() const;
    int zInt() const;
    float length() const;
    Point3f& norm();
    void show() const;
    Point3f& operator+=(const Point3f& r);
    Point3f& operator-=(const Point3f& r);
    Point3f& operator*=(float a);
    Point3f& operator/=(float a);
    std::string toString() const;
};

inline Point3f& Point3f::norm() {
    (*this) /= length();
    return *this;
}

inline void Point3f::show() const {
    glVertex3f(x, y, z);
}

inline Point3f cross(const Point3f& l, const Point3f& r) {
    Point3f ret;
    ret.x = l.y * r.z - l.z * r.y;
    ret.y = l.z * r.x - l.x * r.z;
    ret.z = l.x * r.y - l.y * r.x;
    return ret;
}

inline float dot(const Point3f& l, const Point3f &r) {
    return l.x * r.x + l.y * r.y + l.z * r.z;
}

Point3f operator+(const Point3f& l, const Point3f& r);
Point3f operator-(const Point3f& l, const Point3f& r);
Point3f operator*(const Point3f& l, float a);
Point3f operator/(const Point3f& l, float a);
Point3f operator*(float a, const Point3f &l);
Point3f operator/(float a, const Point3f &l);

std::istream& operator>>(std::istream& in, Point2f& p);
std::istream& operator>>(std::istream& in, Point3f& p);
std::istream& operator>>(std::istream& in, Point3i& p);
std::ostream& operator<<(std::ostream& out, const Point2f& p);
std::ostream& operator<<(std::ostream& out, const Point3f& p);
std::ostream& operator<<(std::ostream& out, const Point3i& p);

#endif
