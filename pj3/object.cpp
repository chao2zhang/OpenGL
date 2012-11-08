#include "object.h"

using namespace std;

void depth(const vector<Point>& p, vector<Triangle>& t, const Point3f& r, const Point3f& v) {
    for (int i = 0; i < t.size(); i++) {
        float a = dot(r - p[t[i].v.x].point, v);
        float b = dot(r - p[t[i].v.y].point, v);
        float c = dot(r - p[t[i].v.z].point, v);
        t[i].depth = a < b ? (a < c ? a : c) : (b < c ? b : c);
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
        for (int j = 0; j < p.size(); j++)
            if (p[j].id == i) {
                p[j].normal = Point3f();
                count = 0;
                for (int k = 0; k < t.size(); k++)
                    if (t[k].v.x == j || t[k].v.y == j || t[k].v.z == j) {
                        Point3f normal = cross(p[t[k].v.y].point - p[t[k].v.x].point,
                                               p[t[k].v.z].point - p[t[k].v.x].point).norm();
                        if (dot(p[t[k].v.x].point - center, normal) < 0)
                            normal *= -1;
                        p[i].normal += normal;
                        count ++;
                    }
                p[j].normal /= count;
            }
    }
}

void sort(const vector<Point>& p, vector<Triangle>& t, const Point3f& ref, const Point3f& view) {
    depth(p, t, ref, view);
    for (int i = 0; i < t.size(); i++)
        for (int j = i + 1; j < t.size(); j++)
            if (t[i].depth > t[j].depth)
                swap(t[i], t[j]);
}

void light(Point& p, float ka, float kd, float ks, const Color3f& iA, const Color3f& iL, const Point3f& lum, const Point3f& ref, int n) {
    p.realColor = iA * ka;
    Point3f l = (lum - p.point).norm();
    Point3f v = (ref - p.point).norm();
    float a = kd * dot(l, p.normal);
    if (a < 0) a = 0;
    float b = ks * dot(2 * p.normal - l, v);
    if (b < 0) b = 0;
    b = pow(b, n);
    float c = 1 / ((ref - p.point).length() + (lum - p.point).length());
    p.realColor += iL * c * (a + b);
}

void light(vector<Point>& p, float ka, float kd, float ks, const Color3f& iA, const Color3f& iL, const Point3f& lum, const Point3f& ref, int n) {
    for (int i = 0; i < p.size(); i++)
        light(p[i], ka, kd, ks, iA, iL, lum, ref, n);
}

template <class T>
void randomize(vector<T> v) {
    for (int i = 0; i < v.size(); i++) {
        int j = i + rand() % (v.size() - i);
        swap(v[i], v[j]);
    }
}

void makePixel(int x, int y, const Color3f& c, GLfloat *pixels, int ppl) {
    if (0 <= x && x < ppl && 0 <= y && y < ppl) {
        pixels[(x * ppl + y) * 3] = c.r;
        pixels[(x * ppl + y) * 3 + 1] = c.g;
        pixels[(x * ppl + y) * 3 + 2] = c.b;
    }
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
    Color3f cIncr = (b.color - a.color) / float(steps);
    float x = SX(a);
    float y = SY(a);
    Color3f c = a.color;
    for (int k = 0; k <= steps; k++) {
        pixel(T(round(x)), T(round(y)), c, pixels, ppl);
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
        pixel(T(x), T(y), c, pixels, ppl);
        x += xIncr; c += cIncr;
    }
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
    begC = arr[0].color;
    endC = arr[0].color;
    begCIncr = (arr[1].color - arr[0].color) / float(SY(arr[1]) - SY(arr[0]));
    endCIncr = (arr[2].color - arr[0].color) / float(SY(arr[2]) - SY(arr[0]));
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
    begC = arr[1].color;
    begCIncr = (arr[2].color - arr[1].color) / float(SY(arr[2]) - SY(arr[1]));
    for (int k = begY; k < endY; k++) {
        horizontalLine(begX, endX, k, begC, endC, pixels, ppl);
        begX += begXIncr; endX += endXIncr;
        begC += begCIncr; endC += endCIncr;
    }
}

#undef SX
#undef SY
#undef T
