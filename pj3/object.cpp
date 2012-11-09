//#define TEST 1

#include "object.h"
#include "utils.h"
#include <iostream>
#include <string>

using namespace std;

string Point::toString() const {
    return format(
        "Point(x=%.3f, y=%.3f, z=%.3f) Normal(x=%.3f, y=%.3f, z=%.3f) Raster(x=%.3f, y=%.3f) Color(r=%.3f, g=%.3f, b=%.3f) RealColor(r=%.3f, g=%.3f, b=%.3f)",
        point.x, point.y, point.z, normal.x, normal.y, normal.z, raster.x, raster.y, color.r, color.g, color.b, realColor.r, realColor.g, realColor.b
    );
}

void projectOrtho(Point& p, int option) {
    switch(option) {
    case PLANE_XY:
        p.raster = Point2f(p.point.x, p.point.y);
        break;
    case PLANE_YZ:
        p.raster = Point2f(p.point.y, p.point.z);
        break;
    case PLANE_XZ:
        p.raster = Point2f(p.point.x, p.point.z);
        break;
    }
}

void projectOrtho(vector<Point>& p, int option) {
    for (int i = 0; i < p.size(); i++)
        projectOrtho(p[i], option);
}

void depth(const vector<Point>& p, vector<Triangle>& t, const Point3f& r, const Point3f& v) {
    Point3f vnorm = v; vnorm.norm();
    for (int i = 0; i < t.size(); i++) {
        float a = dot(r - p[t[i].v.x].point, vnorm);
        float b = dot(r - p[t[i].v.y].point, vnorm);
        float c = dot(r - p[t[i].v.z].point, vnorm);
        t[i].depth = a < b ? (a < c ? a : c) : (b < c ? b : c);
        //this is to consider the average depth of vertices
        t[i].depth += (a + b + c) / 1000;
    }
}

void normal(vector<Point>& p, const vector<Triangle>& t, int n) {
    for (int i = 0; i < n; i++) {
        Point3f center;
        int count = 0;
        for (int j = 0; j < p.size(); j++)
            if (p[j].id == i) {
                center += p[j].point;
                count ++;
            }
        center /= count;
        cout << "Center:" << center << endl;
        for (int j = 0; j < p.size(); j++)
            if (p[j].id == i) {
                p[j].normal = Point3f();
                for (int k = 0; k < t.size(); k++)
                    if (t[k].v.x == j || t[k].v.y == j || t[k].v.z == j) {
                        Point3f normal = cross(p[t[k].v.y].point - p[t[k].v.x].point,
                                               p[t[k].v.z].point - p[t[k].v.x].point);
                        if (dot(p[t[k].v.x].point - center, normal) < 0)
                            normal *= -1;
                        normal.norm();
                        p[j].normal += normal;
                    }
                p[j].normal.norm();
            }
    }
}

void sort(const vector<Point>& p, vector<Triangle>& t, const Point3f& ref, const Point3f& view) {
    depth(p, t, ref, view);
    for (int i = 0; i < t.size(); i++)
        for (int j = i + 1; j < t.size(); j++)
            if (t[i].depth < t[j].depth)
                swap(t[i], t[j]);
}

void light(Point& p, float ka, float kd, float ks, const Color3f& iA, const Color3f& iL, const Point3f& lum, const Point3f& ref, int sp) {
    Color3f ratio = iA * ka;
    Point3f l = (lum - p.point).norm();
    Point3f v = (ref - p.point).norm();
    Point3f r = (2 * p.normal * dot(p.normal, l) - l).norm();
    cout << "Point:" << p.point << " View:" << v << " Lum:" << l << " Reflect:" << r << endl;
    float a = kd * dot(l, p.normal);
    if (a < 0) a = 0;
    float b = dot(r, v);
    if (b < 0) b = 0;
    b = pow(b, sp);
    b *= ks;
    float c = 1 / ((ref - p.point).length() + (lum - p.point).length());
    ratio += iL * c * (a + b);
    cout << "a:" << a << " b:" << b << endl;
    p.realColor.r = p.color.r * ratio.r;
    p.realColor.g = p.color.g * ratio.g;
    p.realColor.b = p.color.b * ratio.b;
}

void light(vector<Point>& p, float ka, float kd, float ks, const Color3f& iA, const Color3f& iL, const Point3f& lum, const Point3f& ref, int sp) {
    for (int i = 0; i < p.size(); i++)
        light(p[i], ka, kd, ks, iA, iL, lum, ref, sp);
}

template <class T>
void randomize(vector<T>& v) {
    for (int i = 0; i < v.size(); i++) {
        int j = i + rand() % (v.size() - i);
        swap(v[i], v[j]);
    }
}

void makePixel(int x, int y, const Color3f& c, GLfloat *pixels, int ppl) {
    if (0 <= x && x < ppl && 0 <= y && y < ppl) {
        pixels[(y * ppl + x) * 3] = c.r;
        pixels[(y * ppl + x) * 3 + 1] = c.g;
        pixels[(y * ppl + x) * 3 + 2] = c.b;
    }
}

void testPixel(int x, int y, const Color3f& c, GLfloat *pixels, int ppl) {
    makePixel(x, y, c, pixels, ppl);
    makePixel(x+1, y, c, pixels, ppl);
    makePixel(x-1, y, c, pixels, ppl);
    makePixel(x, y+1, c, pixels, ppl);
    makePixel(x+1, y+1, c, pixels, ppl);
    makePixel(x-1, y+1, c, pixels, ppl);
    makePixel(x, y-1, c, pixels, ppl);
    makePixel(x+1, y-1, c, pixels, ppl);
    makePixel(x-1, y-1, c, pixels, ppl);
}

void pixel(int x, int y, const Color3f& c, GLfloat *pixels, int ppl) {
    vector<Color3f> v;
    float m = (c.r > c.g ? (c.r > c.b ? c.r : c.b) : (c.g > c.b ? c.g : c.b));
    int countBlack = ceil(9 - 9 * m);
    int countRed = ceil(c.r / (c.r + c.g + c.b) * (9 * m));
    int countGreen = ceil(c.g / (c.r + c.g + c.b) * (9 * m));
    int countBlue = ceil(c.b / (c.r + c.g + c.b) * (9 * m));
    while(countBlack--) v.push_back(Color3f(0, 0, 0));
    while(countRed--) v.push_back(Color3f(1, 0, 0));
    while(countGreen--) v.push_back(Color3f(0, 1, 0));
    while(countBlue--) v.push_back(Color3f(0, 0, 1));
    srand(x * ppl + y);
    randomize(v);
    makePixel(x, y, v[0], pixels, ppl);
    makePixel(x+1, y, v[1], pixels, ppl);
    makePixel(x-1, y, v[2], pixels, ppl);
    makePixel(x, y+1, v[3], pixels, ppl);
    makePixel(x+1, y+1, v[4], pixels, ppl);
    makePixel(x-1, y+1, v[5], pixels, ppl);
    makePixel(x, y-1, v[6], pixels, ppl);
    makePixel(x+1, y-1, v[7], pixels, ppl);
    makePixel(x-1, y-1, v[8], pixels, ppl);
}

#define SX(e) round(e.raster.x * ppl / 3)
#define SY(e) round(e.raster.y * ppl / 3)
#define T(e) (3 * e + 1)

void line(const Point& a, const Point& b, GLfloat* pixels, int ppl) {
    int dx = SX(b) - SX(a);
    int dy = SY(b) - SY(a);
    int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
    float xIncr = float(dx) / float(steps);
    float yIncr = float(dy) / float(steps);
    Color3f cIncr = (b.realColor - a.realColor) / float(steps);
    float x = SX(a);
    float y = SY(a);
    Color3f c = a.realColor;
    for (int k = 0; k <= steps; k++) {
        #ifndef TEST
        pixel(T(round(x)), T(round(y)), c, pixels, ppl);
        #else
        testPixel(T(round(x)), T(round(y)), c, pixels, ppl);
        #endif
        x += xIncr; y += yIncr; c += cIncr;
    }
}

void horizontalLine(int begX, int endX, int y, const Color3f& begC, const Color3f& endC, GLfloat* pixels, int ppl) {
    int steps = endX - begX;
    if (steps < 0)
        steps = -steps;
    int xIncr = 1;
    if (endX < begX)
        xIncr = -1;
    Color3f cIncr = (endC - begC) / steps;
    Color3f c = begC;
    int x = begX;
    for (int k = 0; k <= steps; k++) {
        #ifndef TEST
        pixel(T(x), T(y), c, pixels, ppl);
        #else
        testPixel(T(x), T(y), c, pixels, ppl);
        #endif
        x += xIncr; c += cIncr;
    }
}

void triangle(const vector<Point>& p, const vector<Triangle>& t, GLfloat* pixels, int ppl) {
    for (int i = 0; i < t.size(); i++)
        triangle(p[t[i].v.x], p[t[i].v.y], p[t[i].v.z], pixels, ppl);
}

void triangle(const Point& a, const Point& b, const Point& c, GLfloat* pixels, int ppl) {
    Point arr[] = {a, b, c};
    if (arr[0].raster.y > arr[1].raster.y) swap(arr[0], arr[1]);
    if (arr[0].raster.y > arr[2].raster.y) swap(arr[0], arr[2]);
    if (arr[1].raster.y > arr[2].raster.y) swap(arr[1], arr[2]);
    line(arr[0], arr[1], pixels, ppl);
    line(arr[0], arr[2], pixels, ppl);
    line(arr[1], arr[2], pixels, ppl);
    float begX, endX;
    float begXIncr, endXIncr;
    Color3f begC, endC;
    Color3f begCIncr, endCIncr;
    int begY, endY;
    //Phase 1, bottom -> middle
    begY = SY(arr[0]);
    endY = SY(arr[1]);
    begX = SX(arr[0]);
    endX = SX(arr[0]);
    begXIncr = float(SX(arr[1]) - SX(arr[0])) / float(SY(arr[1]) - SY(arr[0]));
    endXIncr = float(SX(arr[2]) - SX(arr[0])) / float(SY(arr[2]) - SY(arr[0]));
    begC = arr[0].realColor;
    endC = arr[0].realColor;
    begCIncr = (arr[1].realColor - arr[0].realColor) / float(SY(arr[1]) - SY(arr[0]));
    endCIncr = (arr[2].realColor - arr[0].realColor) / float(SY(arr[2]) - SY(arr[0]));
    for (int k = begY; k < endY; k++) {
        horizontalLine(begX, endX, k, begC, endC, pixels, ppl);
        begX += begXIncr; endX += endXIncr;
        begC += begCIncr; endC += endCIncr;
    }
    //Phase 2, middle -> top
    begY = SY(arr[1]);
    endY = SY(arr[2]);
    begX = SX(arr[1]);
    begXIncr = float(SX(arr[2]) - SX(arr[1])) / float(SY(arr[2]) - SY(arr[1]));
    begC = arr[1].realColor;
    begCIncr = (arr[2].realColor - arr[1].realColor) / float(SY(arr[2]) - SY(arr[1]));
    for (int k = begY; k < endY; k++) {
        horizontalLine(begX, endX, k, begC, endC, pixels, ppl);
        begX += begXIncr; endX += endXIncr;
        begC += begCIncr; endC += endCIncr;
    }
}

#undef SX
#undef SY
#undef T

string toString(const vector<Point>& p, const vector<Triangle>& t, int num) {
    string ret;
    ret += format("Object #%d:\n", num);
    for (int i = 0; i < p.size(); i++)
        ret += format("Point #%d = (%.3f, %.3f, %.3f);", i, p[i].point.x, p[i].point.y, p[i].point.z);
    ret += "\n";
    for (int i = 0; i < p.size(); i++)
        ret += format("Triangle #%d = (%.3f, %.3f, %.3f);", i, t[i].v.x, t[i].v.y, t[i].v.z);
    ret += "\n";
}
