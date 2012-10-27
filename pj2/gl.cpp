#include "gl.h"
#include <cstring>
#include <cmath>
#include <cstdio>
#include <cstdarg>

using namespace std;

istream& operator>>(istream& in, Point2f& p) {
    in >> p.x >> p.y;
    return in;
}

ostream& operator<<(ostream& out, const Point2f& p) {
    out << p.x << ' ' << p.y;
    return out;
}

istream& operator>>(istream& in, Point3f& p) {
    in >> p.x >> p.y >> p.z;
    return in;
}

ostream& operator<<(ostream& out, const Point3f& p) {
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

Line::Line(int x0, int y0, int x1, int y1) {
    a = y1 - y0;
    b = x0 - x1;
    c = - a * x0 - b * y0;
}

Line::Line(const Point2f& l, const Point2f& r) {
    a = r.yInt() - l.yInt();
    b = l.xInt() - r.xInt();
    c = - a * l.xInt() - b * l.yInt();
}

float area(const Point2f& l, const Point2f& r) {
    return l.x * r.y - l.y * r.x;
}

bool intersection(const Line& l, const Line& r, Point2f& i) {
    float det = l.a * r.b - l.b * r.a;
    if (abs(det) < 0.001)
        return -1;
    i.x = ((-l.c) * r.b - l.b * (-r.c)) / det;
    i.y = (l.a * (-r.c) - (-l.c) * r.a) / det;
    printf("(%.1fx+%.1fy+%.1f = 0), (%.1fx+%.1fy+%.1f = 0), (%.1f, %.1f)\n", l.a, l.b, l.c, r.a, r.b, r.c, i.x, i.y);
    return 0;
}

std::string format(const std::string &fmt, ...) {
    int size=100;
    std::string str;
    va_list ap;
    while (1) {
        str.resize(size);
        va_start(ap, fmt);
        int n = vsnprintf((char *)str.c_str(), size, fmt.c_str(), ap);
        va_end(ap);
        if (n > -1 && n < size) {
            str.resize(n);
            return str;
        }
        if (n > -1)
            size=n+1;
        else
            size*=2;
    }
}

void print(void* font, const std::string& str, Point2i& p, const Point2i& window, int h){
     glRasterPos2i(p.x, p.y);
     int dx = glutBitmapWidth(font, ' ');
     int c = 0, s = 0;
     for (int i = 0; i < str.size(); i++) {
        if (dx + p.x > window.x || str[i] == '\n') {
            p.x = 0; p.y -= h; c = 0;
            glRasterPos2i(p.x, p.y);
        }
        switch (str[i]) {
            case '\n':
                break;
            case '\t':
                s = (c / 4 + 1) * 4 - c;
                while (s--) {
                    p.x += dx; ++c;
                    glutBitmapCharacter(font, ' ');
                }
                break;
            default:
                p.x += dx; ++c;
                glutBitmapCharacter(font, str[i]);
                break;
        }
     }
}

