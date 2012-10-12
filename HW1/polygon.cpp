#include "polygon.h"
#include <cmath>

using namespace std;

void Polygon::push(float x, float y) {
    push(Point(x, y));
    validCentroid = false;
}

void Polygon::push(const Point& p) {
    list_.push_back(p);
}

void Polygon::genCentroid() {
    if (list_.size() == 1) {
        centroid_ = list_[0];
        return;
    }
    if (list_.size() == 2) {
        centroid_.x = (list_[0].x + list_[1].x) / 2;
        centroid_.y = (list_[0].y + list_[1].y) / 2;
        return;
    }
    int i;
    float a = 0.0;
    centroid_.x = 0;
    centroid_.y = 0;
    for (i = 0; i < list_.size() - 1; i++) {
        a += area(list_[i], list_[i + 1]);
        centroid_.x += (list_[i].x + list_[i + 1].x) * area(list_[i], list_[i + 1]);
        centroid_.y += (list_[i].y + list_[i + 1].y) * area(list_[i], list_[i + 1]);
    }
    a += area(list_[i], list_[0]);
    centroid_.x += (list_[i].x + list_[0].x) * area(list_[i], list_[0]);
    centroid_.y += (list_[i].y + list_[0].y) * area(list_[i], list_[0]);
    centroid_.x /= 3 * a;
    centroid_.y /= 3 * a;
    validCentroid = true;
}

void Polygon::draw(GLubyte* pixels) const{
    if (list_.size() >= 2) {
        int i;
        for (i = 0; i < list_.size() - 1; i++)
            makeLine(list_[i], list_[i+1], pixels);
        makeLine(list_[i], list_[0], pixels);
    }
}

void Polygon::translate(float x, float y) {
    for (int i = 0; i < list_.size(); i++) {
        list_[i].x += x;
        list_[i].y += y;
    }
    centroid_.x += x;
    centroid_.y += y;
}

void Polygon::translate(const Point& p) {
    translate(p.x, p.y);
}

void Polygon::rotate(float a) {
    centroid();
    for (int i = 0; i < list_.size(); i++) {
        Point p = list_[i];
        list_[i].x = (p.x - centroid_.x) * cos(a) - (p.y - centroid_.y) * sin(a) + centroid_.x;
        list_[i].y = (p.x - centroid_.x) * sin(a) + (p.y - centroid_.y) * cos(a) + centroid_.y;
    }
}

void Polygon::scale(float a) {
    centroid();
    for (int i = 0; i < list_.size(); i++) {
        Point p = list_[i];
        list_[i].x = (p.x - centroid_.x) * a + centroid_.x;
        list_[i].y = (p.y - centroid_.y) * a + centroid_.y;
    }
}

const Point& Polygon::centroid() {
    if (!validCentroid)
        genCentroid();
    return centroid_;
}

const vector<Point>& Polygon::list() const {
    return list_;
}
