#ifndef POINT_H
#define POINT_H

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
    Point2f& operator+=(const Point2f& r);
    Point2f& operator-=(const Point2f& r);
    Point2f& operator*=(float a);
    Point2f& operator/=(float a);
};

Point2f operator+(const Point2f& l, const Point2f& r);
Point2f operator-(const Point2f& l, const Point2f& r);
Point2f operator*(const Point2f& l, float a);
Point2f operator/(const Point2f& l, float a);
Point2f operator*(float a, const Point2f &l);
Point2f operator/(float a, const Point2f &l);

#endif
