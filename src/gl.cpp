#include "gl.h"
#include <cstring>
#include <cmath>
#include <cstdio>

using namespace std;

Color GColor;
Size GWindow;
Rect GViewport(0, 0, GWindow.width, GWindow.height);
void (*makeLine)(int x1, int y1, int x2, int y2, GLubyte* pixels);

bool Rect::inside(int x, int y) const {
    return left <= x && x < right && bottom <= y && y < top;
}

istream& operator>>(istream& in, Point& p) {
    in >> p.x >> p.y;
    return in;
}

ostream& operator<<(ostream& out, const Point& p) {
    out << p.x << ' ' << p.y;
    return out;
}

int Point::xInt() const {
    return round(x);
}

int Point::yInt() const {
    return round(y);
}

Line::Line(int x0, int y0, int x1, int y1) {
    a = y1 - y0;
    b = x0 - x1;
    c = - a * x0 - b * y0;
}

Line::Line(const Point& l, const Point& r) {
    a = r.yInt() - l.yInt();
    b = l.xInt() - r.xInt();
    c = - a * l.xInt() - b * l.yInt();
}

float area(const Point& l, const Point& r) {
    return l.x * r.y - l.y * r.x;
}

bool intersection(const Line& l, const Line& r, Point& i) {
    float det = l.a * r.b - l.b * r.a;
    if (abs(det) < 0.001)
        return -1;
    i.x = ((-l.c) * r.b - l.b * (-r.c)) / det;
    i.y = (l.a * (-r.c) - (-l.c) * r.a) / det;
    printf("(%.1fx+%.1fy+%.1f = 0), (%.1fx+%.1fy+%.1f = 0), (%.1f, %.1f)\n", l.a, l.b, l.c, r.a, r.b, r.c, i.x, i.y);
    return 0;
}

void clearPixel(GLubyte *pixels, size_t size) {
    memset(pixels, 0xff, size);
}

void makeColor(GLubyte r, GLubyte g, GLubyte b) {
    GColor.r = r;
    GColor.g = g;
    GColor.b = b;
}

void makeColor(const Color& c) {
    GColor = c;
}

void makePixel(int x, int y, GLubyte* pixels) {
    if (0 <= x && x < GWindow.width && 0 <= y && y < GWindow.height) {
        int position = (x + y * GWindow.width) * 3;
        pixels[position] = GColor.r;
        pixels[position + 1] = GColor.g;
        pixels[position + 2] = GColor.b;
    }
}

void makeLineByPoint(const Point& a, const Point& b, GLubyte *pixels) {
    makeLine(a.xInt(), a.yInt(), b.xInt(), b.yInt(), pixels);
}

void makeLineBres(int x1, int y1, int x2, int y2, GLubyte* pixels) {
    int dx = abs(x2 - x1), dy = abs(y2 - y1);
    int s = (x2 - x1) ^ (y2 - y1);
    if (dy < dx) {
        int p = 2 * dy - dx;
        int tDy = 2 * dy, tDyMDx = 2 * (dy - dx);
        int x, y;
        if (x1 > x2) {
            x = x2;
            y = y2;
            x2 = x1;
        } else {
            x = x1;
            y = y1;
        }
        makePixel(x, y, pixels);
        if (s >= 0)
            while (x < x2) {
                ++x;
                if (p < 0)
                    p += tDy;
                else {
                    ++y;
                    p += tDyMDx;
                }
                makePixel(x, y, pixels);
            }
        else
            while (x < x2) {
                ++x;
                if (p < 0)
                    p += tDy;
                else {
                    --y;
                    p += tDyMDx;
                }
                makePixel(x, y, pixels);
            }
    } else {
        int p = 2 * dx - dy;
        int tDx = 2 * dx, tDxMDy = 2 * (dx - dy);
        int x, y;
        if (y1 > y2) {
            x = x2;
            y = y2;
            y2 = y1;
        } else {
            x = x1;
            y = y1;
        }
        makePixel(x, y, pixels);
        if (s >= 0)
            while (y < y2) {
                ++y;
                if (p < 0)
                    p += tDx;
                else {
                    ++x;
                    p += tDxMDy;
                }
                makePixel(x, y, pixels);
            }
        else {
            while (y < y2) {
                ++y;
                if (p < 0)
                    p += tDx;
                else {
                    --x;
                    p += tDxMDy;
                }
                makePixel(x, y, pixels);
            }
        }
    }
}

void makeLineDDA(int x1, int y1, int x2, int y2, GLubyte* pixels) {
    int dx = x2 - x1, dy = y2 - y1;
    int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy), k;
    float xIncr, yIncr, x = x1, y = y1;
    xIncr = float(dx) / float(steps);
    yIncr = float(dy) / float(steps);
    makePixel(round(x), round(y), pixels);
    for (k = 0; k < steps; k++) {
        x += xIncr; y += yIncr;
        makePixel(round(x), round(y), pixels);
    }
}

void makeViewport(int left, int bottom, int right, int top) {
    GViewport.left = left;
    GViewport.bottom = bottom;
    GViewport.right = right;
    GViewport.top = top;
}

void projViewport(GLubyte* pixels) {
    Color tmp = GColor;
    makeColor(0, 0, 0);
    for (int x = 0; x < GWindow.width; x++)
        for (int y = 0; y < GWindow.height; y++)
            if (!(GViewport.left <= x && x < GViewport.right && GViewport.bottom <= y && y < GViewport.top))
                makePixel(x, y, pixels);
    GColor = tmp;
}

