#include "curve.h"
#include <iostream>
#include <stdexcept>
#include <cstdlib>
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

void Bezier::show() const{
    for (float t = 0; t <= 1; t += 0.001)
        show(t);
}

int Bspline::knotSize() const {
    return knots.size();
}

void Bspline::setKnot(int pos, float k) {
    if (0 <= pos && pos < knots.size())
        knots[pos] = k;
    else
        throw runtime_error("Index out of bound");
}

float Bspline::getKnot(int pos) const {
    if (0 <= pos && pos < knots.size())
        return knots[pos];
    else
        throw runtime_error("Index out of bound");
}

void Bspline::addKnot() {
    float delta;
    if (knots.size() <= 1)
        delta = 1.0;
    else
        delta = (knots.back() - knots.front()) / (knots.size() - 1);
    knots.push_back(knots.size() ? knots.back() + delta : 0.0);
}

void Bspline::setK(int vk) {
    if ((2 <= vk && vk <= points.size()) || points.size() < 2) {
        float delta;
        while (k < vk) {
            addKnot();
            k++;
        }
        while (k > vk) {
            knots.pop_back();
            k--;
        }
    }
}

void Bspline::addK(int dk) {
    setK(k + dk);
}

int Bspline::getK() const {
    return k;
}

void Bspline::add(const Point2f& p) {
    Curve::add(p);
    addKnot();
}

void Bspline::insert(int pos, const Point2f& p) {
    Curve::insert(pos, p);
    addKnot();
}

void Bspline::remove(int pos) {
    Curve::remove(pos);
    knots.pop_back();
}

void Bspline::show(float u) const {
    if (!points.size()) return;
    int bigI;
    bool found = false;
    for (bigI = k - 1; bigI < points.size(); bigI++)
        if (knots[bigI] <= u && u < knots[bigI + 1]) {
            found = true;
            break;
        }
    if (!found) throw runtime_error("Knot out of range");
    vector<Point2f> last;
    int beg = bigI - k + 1;
    int end = beg + k;
    vector<Point2f> now(
        points.begin() + beg,
        points.begin() + end);
    for (int r = 0; r < k - 1; r++) {
        last.assign(now.begin(), now.end());
        now.clear();
        for (int i = 0; i < last.size() - 1; i++) {
            int leftIndex = bigI - k + i + r + 2;
            int rightIndex = bigI + 1 + i;
            float left = knots[leftIndex];
            float right = knots[rightIndex];
            now.push_back(((right - u) * last[i] + (u - left) * last[i+1]) / (right - left));
        }
    }
    glVertex2f(now.front().x, now.front().y);
}

void Bspline::show() const {
    if (knots.size()) {
        float delta = (knots[points.size()] - knots[k - 1]) / 1000;
        for (float u = knots[k - 1]; u < knots[points.size()]; u += delta)
            show(u);
    }
}
