#include "curve.h"
#include <iostream>
#include <stdexcept>
#include <GL/glut.h>

using namespace std;

int Curve::size() const {
    return points.size();
}

void Curve::set(int pos, const Point2f& p) {
    if (0 <= pos && pos < points.size())
        points[pos] = p;
    else
        throw runtime_error("Index out of bound");
}

void Curve::add(const Point2f& p) {
    points.push_back(p);
}

void Curve::insert(int pos, const Point2f& p) {
    if (0 <= pos && pos < points.size()) {
        points.insert(points.begin() + pos, p);
    }
    else
        throw runtime_error("Index out of bound");
}

void Curve::remove(int pos) {
    if (0 <= pos && pos < points.size()) {
        points.erase(points.begin() + pos);
    }
    else
        throw runtime_error("Index out of bound");
}

const Point2f& Curve::get(int pos) const{
    if (0 <= pos && pos < points.size())
        return points[pos];
    else
        throw runtime_error("Index out of bound");
}

void Bezier::show(float t) const {
    if (points.empty()) return;
    int time = points.size() - 1;
    vector<Point2f> last, now(points.begin(), points.end());
    while(time--) {
        last.assign(now.begin(), now.end());
        now.clear();
        for (int i = 0; i < last.size() - 1; i++)
            now.push_back(t * last[i] + (1-t) * last[i+1]);
    }
    glVertex2f(now.front().x, now.front().y);
}
