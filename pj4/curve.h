#ifndef CURVE_H
#define CURVE_H

#include "point.h"
#include <vector>

using namespace std;

class Curve {
protected:
    vector<Point2f> points;
public:
    int size() const;
    void set(int pos, const Point2f& p);
    void add(const Point2f& p);
    void insert(int pos, const Point2f& p);
    void remove(int pos);
    const Point2f& get(int pos) const;
    virtual void show(float t) const = 0;
};

class Bezier : public Curve {
    void show(float t) const;
};

#endif
