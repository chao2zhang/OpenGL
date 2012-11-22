#ifndef CURVE_H
#define CURVE_H

#include "point.h"
#include <vector>
#include <string>

using namespace std;

class Curve {
protected:
    vector<Point2f> points;
public:
    int size() const;
    virtual void set(int pos, const Point2f& p);
    virtual void add(const Point2f& p);
    virtual void insert(int pos, const Point2f& p);
    virtual void remove(int pos);
    const Point2f& get(int pos) const;
    virtual void show() const = 0;
    virtual const string name() const = 0;
};

#define BEZIER_NAME "Bezier"

class Bezier : public Curve {
private:
    void show(float t) const;
public:
    void show() const;
    const string name() const {
        return BEZIER_NAME;
    }
};

#define BSPLINE_NAME "Bspline"

class Bspline: public Curve {
protected:
    vector<float> knots;
    int k;
private:
    void addKnot();
    void show(float u) const;
public:
    Bspline():Curve(), k(0){setK(2);}
    int knotSize() const;
    void add(const Point2f& p);
    void insert(int pos, const Point2f& p);
    void remove(int pos);
    void setKnot(int pos, float k);
    float getKnot(int pos) const;
    float frontKnot() const;
    float backKnot() const;
    void setK(int k);
    void addK(int dk);
    int getK() const;
    const string name() const {
        return BSPLINE_NAME;
    }
public:
    void show() const;
};

#endif
