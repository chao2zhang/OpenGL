#ifndef OBJECT_H
#define OBJECT_H

#include "base.h"
#include "color.h"
#include <vector>

using namespace std;

extern float iA, iL, kK;
extern Point3f kA, kD, kS;
extern int bigN;
extern float bigK;
extern float kT, kR;
extern float eta;
extern Point3f lightSource;

class Line {
public:
    Point3f start;
    Point3f u;
    bool inside;
public:
    Line(const Point3f& start_, const Point3f& u_, bool inside_):
        start(start_), u(u_), inside(inside_){}
    Line():inside(false){}
};

class Primitive {
public:
    bool transparent;
    Color3f color;
public:
    virtual bool intersect(const Line& l, Point3f& result) = 0;
    virtual Point3f normal(const Point3f& p) = 0;
};

class Polygon : public Primitive {
public:
    vector<Point3f> v;
    Point3f n;
public:
    bool intersect(const Line& l, Point3f& result);
    Point3f normal(const Point3f& p);
};

class Sphere : public Primitive {
public:
    Point3f center;
    float radius;
public:
    bool intersect(const Line& l, Point3f& result);
    Point3f normal(const Point3f& p);
};

class Quadratic : public Primitive {
public:
    float A, B, C, D, E, F, G, H, I, J;
public:
    bool intersect(const Line& l, Point3f& result);
    Point3f normal(const Point3f& p);
};

inline void makePixel(int x, int y, const Color3f& c, GLfloat *pixels, int resolution) {
    if (0 <= x && x < resolution && 0 <= y && y < resolution) {
        pixels[(y * resolution + x) * 3] = c.r;
        pixels[(y * resolution + x) * 3 + 1] = c.g;
        pixels[(y * resolution + x) * 3 + 2] = c.b;
    }
}
inline void normalize(GLfloat *pixels, int resolution) {
    GLfloat best = 0.0;
    for (int i = 0; i < resolution * resolution * 3; i++) {
        if (pixels[i] > best)
            best = pixels[i];
    }
    if (best > 0.0) {
        for (int i = 0; i < resolution * resolution * 3; i++)
            pixels[i] /= best;
    }
}

Polygon* newTriangle(const Point3f& v1, const Point3f& v2, const Point3f& v3, const Point3f& normal);
Polygon* newSquare(const Point3f& v1, const Point3f& v2, const Point3f& v3, const Point3f& v4, const Point3f& normal);
Quadratic* newEllipsoid(const Point3f& center, float a, float b, float c);
Quadratic* newSphere(const Point3f& center, float radius);
Point3f refract(const Point3f& u, const Point3f& normal);
Point3f reflect(const Point3f& u, const Point3f& normal);
Point3f phong(const Point3f& point, const Point3f& ref, const Point3f& normal, const vector<Primitive*>& primitives);
bool intersect(const Line& line, const vector<Primitive*>& primitives, Point3f& result, int& resultId);
Color3f light(const Line& line, const vector<Primitive*>& primitives, int depth);

#endif
