#ifndef POINT_H
#define POINT_H

#include <GL/glut.h>
#include <cmath>
#include <iostream>

#define PI 3.141593
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
    float length() const;
    Point3f& operator+=(const Point3f& r);
    Point3f& operator-=(const Point3f& r);
    Point3f& operator*=(float a);
    Point3f& operator/=(float a);
    void project2D(ProjType p, const Point2i& view) const;
    void projectCabinet(const Point2i& view) const;
    void projectCavalier(const Point2i& view) const;
    void show() const;
};

inline void Point3f::project2D(ProjType p, const Point2i& view) const {
    switch(p) {
        case PROJ_XY:
            glVertex2f(x * view.x, y * view.y);
            break;
        case PROJ_YZ:
            glVertex2f(y * view.x, z * view.y);
            break;
        case PROJ_XZ:
            glVertex2f(x * view.x, z * view.y);
            break;
        default:
            break;
    }
}

inline void Point3f::projectCabinet(const Point2i& view) const {
    glVertex2f(
        (x - CABINET_INV * z) * view.x,
        (y - CABINET_INV * z) * view.y
    );
}

inline void Point3f::projectCavalier(const Point2i& view) const {
    glVertex2f(
        (x - CAVALIER_INV * z) * view.x,
        (y - CAVALIER_INV * z) * view.y
    );
}

inline void Point3f::show() const {
    glVertex3f(x, y, z);
}

Point3f operator+(const Point3f& l, const Point3f& r);
Point3f operator-(const Point3f& l, const Point3f& r);
Point3f operator*(const Point3f& l, float a);
Point3f operator/(const Point3f& l, float a);

std::istream& operator>>(std::istream& in, Point2f& p);
std::ostream& operator<<(std::ostream& out, const Point2f& p);
std::istream& operator>>(std::istream& in, Point3f& p);
std::ostream& operator<<(std::ostream& out, const Point3f& p);

#endif
