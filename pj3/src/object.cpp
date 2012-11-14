#include "object.h"
#include "utils.h"
#include "matrix.h"
#include <iostream>
#include <string>

bool grayScale = false;
bool halfTone = true;
float iA = 0.2, iL = 1, kK = 1;
Point3f kA(1, 1, 1), kD(1, 1, 1), kS(1, 1, 1);
int pN = 3;
float dist = 10;
Point3f lightSource(0.5, 0.5, 1);

using namespace std;

Color3f maxColor(const vector<Point>& p) {
    Color3f ret;
    for (int i = 0; i < p.size(); i++) {
        if (p[i].realColor.r > ret.r) ret.r = p[i].realColor.r;
        if (p[i].realColor.g > ret.g) ret.g = p[i].realColor.g;
        if (p[i].realColor.b > ret.b) ret.b = p[i].realColor.b;
    }
    return ret;
}

void translate(vector<Point>& p, const Point3f& diff, int id) {
    for (int i = 0; i < p.size(); i++)
        if (p[i].id == id)
            p[i].point += diff;
}

void rotate(vector<Point>& p, const Point3f& r1, const Point3f& r2, float a, int id) {
    Point3f u = (r2 - r1).norm();
    float cosa = cos(a / 180 * PI);
    float sina = sin(a / 180 * PI);
    float onec = 1 - cosa;
    Matrix rot(3, 3);
    rot[0][0] = u.x * u.x * onec + cosa;
    rot[0][1] = u.x * u.y * onec - u.z * sina;
    rot[0][2] = u.x * u.z * onec + u.y * sina;
    rot[1][0] = u.y * u.x * onec + u.z * sina;
    rot[1][1] = u.y * u.y * onec + cosa;
    rot[1][2] = u.z * u.y * onec + u.x * sina;
    rot[2][0] = u.x * u.z * onec + u.y * sina;
    rot[2][1] = u.y * u.z * onec - u.x * sina;
    rot[2][2] = u.z * u.z * onec + cosa;
    for (int i = 0; i < p.size(); i++)
        if (p[i].id == id) {
            Point3f t = p[i].point - r1;
            Matrix v(3, 1);
            v[0][0] = t.x; v[1][0] = t.y; v[2][0] = t.z;
            Matrix r = rot * v;
            t.x = r[0][0]; t.y = r[1][0]; t.z = r[2][0];
            p[i].point = t + r1;
        }
}

void scale(vector<Point>& p, float a, int id) {
    Point3f center;
    int count = 0;
    for (int j = 0; j < p.size(); j++)
        if (p[j].id == id) {
            center += p[j].point;
            count ++;
        }
    center /= count;
    for (int j = 0; j < p.size(); j++)
        if (p[j].id == id) {
            p[j].point -= center;
            p[j].point *= a;
            p[j].point += center;
        }
}


void scale(vector<Point>& p, const Point3f& center, float a) {
    for (int j = 0; j < p.size(); j++) {
        p[j].point -= center;
        p[j].point *= a;
        p[j].point += center;
    }
}

string Point::toString() const {
    return format(
        "Point(%.3f, %.3f, %.3f) Normal(%.3f, %.3f, %.3f) Color(%.3f, %.3f, %.3f) RealColor(%.3f, %.3f, %.3f)",
        point.x, point.y, point.z, normal.x, normal.y, normal.z, color.r, color.g, color.b, realColor.r, realColor.g, realColor.b
    );
}

string Triangle::toString() const {
    return format(
        "(%d, %d, %d)",
        v.x, v.y, v.z
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
    case PLANE_YX:
        p.raster = Point2f(p.point.y, p.point.x);
        break;
    case PLANE_ZX:
        p.raster = Point2f(p.point.z, p.point.x);
        break;
    case PLANE_ZY:
        p.raster = Point2f(p.point.z, p.point.y);
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

void light(Point& p, const Point3f& lum, const Point3f& ref) {
    Point3f ratio = kA * iA;
    Point3f l = (lum - p.point).norm();
    Point3f v = (ref - p.point).norm();
    Point3f r = (2 * p.normal * dot(p.normal, l) - l).norm();
    //cout << "Point:" << p.point << " View:" << v << " Lum:" << l << " Reflect:" << r << endl;
    Point3f a = dot(l, p.normal) * kD;
    if (a.x < 0) a.x = 0;
    if (a.y < 0) a.y = 0;
    if (a.z < 0) a.z = 0;
    Point3f b = dot(r, v) * kS;
    if (b.x < 0) b.x = 0; b.x = pow(b.x, pN);
    if (b.y < 0) b.y = 0; b.y = pow(b.y, pN);
    if (b.z < 0) b.z = 0; b.z = pow(b.z, pN);
    float c = 1 / ((ref - p.point).length() + kK * (lum - p.point).length());
    ratio += iL * c * (a + b);
    p.realColor.r = p.color.r * ratio.x;
    p.realColor.g = p.color.g * ratio.y;
    p.realColor.b = p.color.b * ratio.z;
}

void light(vector<Point>& p, const Point3f& lum, const Point3f& ref) {
    for (int i = 0; i < p.size(); i++)
        light(p[i], lum, ref);
    Color3f c = maxColor(p);
    for (int i = 0; i < p.size(); i++) {
        p[i].realColor.r /= c.r;
        p[i].realColor.g /= c.g;
        p[i].realColor.b /= c.b;
    }
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

void pixel(int x, int y, const Color3f& c, GLfloat *pixels, int ppl) {
    if (halfTone) {
        if (grayScale) {
            vector<Color3f> v;
            float m = c.gray().r;
            int countBlack = ceil(9 - 9 * m);
            int countWhite = ceil(9 * m);
            while(countBlack--) v.push_back(Color3f(0, 0, 0));
            while(countWhite--) v.push_back(Color3f(1, 1, 1));
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
        } else {
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
    } else {
        if (grayScale) {
            Color3f g = c.gray();
            makePixel(x, y, g, pixels, ppl);
            makePixel(x+1, y, g, pixels, ppl);
            makePixel(x-1, y, g, pixels, ppl);
            makePixel(x, y+1, g, pixels, ppl);
            makePixel(x+1, y+1, g, pixels, ppl);
            makePixel(x-1, y+1, g, pixels, ppl);
            makePixel(x, y-1, g, pixels, ppl);
            makePixel(x+1, y-1, g, pixels, ppl);
            makePixel(x-1, y-1, g, pixels, ppl);
        } else {
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
    }
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

string toString(const vector<Point>& p, const vector<Triangle>& t, int curr) {
    string ret;
    ret += format("Object #%d:\n", curr);
    for (int i = 0; i < p.size(); i++)
        if (p[i].id == curr) {
            ret += format("\tVertex #%d = %s\n", i, p[i].toString().c_str());
        }
    ret += "\n";
    for (int i = 0; i < t.size(); i++)
        if (t[i].id == curr) {
            ret += format("\tTriangle #%d = %s\n", i, t[i].toString().c_str());
        }
    ret += "\n";
    return ret;
}
