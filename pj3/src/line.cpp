#include "line.h"

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
