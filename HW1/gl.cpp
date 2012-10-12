#include "gl.h"
#include <cstring>
#include <cmath>

using namespace std;

Color GColor;
Size GWindow;

istream& operator>>(istream& in, Point& p) {
    in >> p.x >> p.y;
    return in;
}

ostream& operator<<(ostream& out, const Point& p) {
    out << p.x << ' ' << p.y;
    return out;
}

float area(const Point& l, const Point& r) {
    return l.x * r.y - l.y * r.x;
}

void cleanPixel(GLubyte *pixels) {
    memset(pixels, 0, sizeof(pixels));
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
    int position = (x + y * GWindow.width) * 3;
    if (0 <= position && position < GWindow.size() * 3) {
        pixels[position] = GColor.r;
        pixels[position + 1] = GColor.g;
        pixels[position + 2] = GColor.b;
    }
}

void makeLine(int x1, int y1, int x2, int y2, GLubyte* pixels) {
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

void makeLine(const Point& a, const Point& b, GLubyte *pixels) {
    makeLine(round(a.x), round(a.y), round(b.x), round(b.y), pixels);
}

void makeLineDDA(int x1, int y1, int x2, int y2, GLubyte* pixels) {
    int dx = x2 - x1, dy = y2 - x2, steps, k;
    float xIncr, yIncr, x = x1, y = y1;
    if (abs(dx) > abs(dy))
        steps = abs(dx);
    else
        steps = abs(dy);
    xIncr = float(dx) / float(steps);
    yIncr = float(dy) / float(steps);
    makePixel(round(x), round(y), pixels);
    for (k = 0; k < steps; k++) {
        x += xIncr; y += yIncr;
        makePixel(round(x), round(y), pixels);
    }
}

void makeLineDDA(const Point& a, const Point& b, GLubyte *pixels) {
    makeLineDDA(round(a.x), round(a.y), round(b.x), round(b.y), pixels);
}
