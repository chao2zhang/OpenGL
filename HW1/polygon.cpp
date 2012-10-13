#include "polygon.h"
#include <cmath>
#include <algorithm>

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
    if (list_.size() > 2) {
        int i;
        for (i = 0; i < list_.size() - 1; i++)
            makeLineByPoint(list_[i], list_[i+1], pixels);
        makeLineByPoint(list_[i], list_[0], pixels);
    } else if (list_.size() == 2) {
        makeLineByPoint(list_[0], list_[1], pixels);
    } else if (list_.size() == 1) {
        makePixel(list_[0].xInt(), list_[0].yInt(), pixels);
    }
}

void Polygon::fill(GLubyte* pixels) const{
    vector<int>* lines = new vector<int>[GWindow.height];
    for (int i = 0; i < list_.size(); i++) {
        const Point& n = list_[i];
        const Point& nn = i == list_.size() - 1 ? list_[0] : list_[i + 1];
        const Point& p = i == 0 ? list_[list_.size() - 1] : list_[i - 1];
        int dx = nn.xInt() - n.xInt(), dy = nn.yInt() - n.yInt(), iy;
        int steps = abs(dy), k;
        float xIncr, x = n.x;
        int yIncr, y = n.yInt();
        xIncr = float(dx) / float(steps);
        yIncr = float(dy) / float(steps);
        for (k = 0; k < steps - 1; k++) {
            x += xIncr; y += yIncr;
            if (0 <= y && y < GWindow.height)
                lines[y].push_back(int(round(x)));
        }
        if (0 <= n.yInt() && n.yInt() < GWindow.height) {
            if ((p.yInt() - n.yInt()) * (nn.yInt() - n.yInt()) < 0)
                lines[n.yInt()].push_back(n.xInt());
            else if (nn.yInt() == n.yInt()) {
                //Boundary case(Horizontal edges
                const Point& nnn = list_[(i + 2) % list_.size()];
                if ((nnn.yInt() - n.yInt()) * (p.yInt() - n.yInt()) < 0)
                    lines[n.yInt()].push_back(n.xInt());
            }
        }
    }
    for (int i = 0; i < GWindow.height; i++)
        if (lines[i].size()) {
            /*
             * For Debug
            if (lines[i].size() % 2 == 1) {
                cout << "No:" << list_.size() << ',' << i << ':';
                for (int j = 0; j < lines[i].size(); j++)
                    cout << lines[i][j] << ' ';
                cout << endl;
            }
             */
            sort(lines[i].begin(), lines[i].end());
            for (int j = 0; j < lines[i].size() - 1; j+=2)
                for (int k = lines[i][j]; k <= lines[i][j+1]; k++)
                    makePixel(k, i, pixels);
        }
    delete [] lines;
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

static bool insideLeft(const Point& p) {return p.x >= GViewport.left;}
static bool insideBottom(const Point& p) {return p.y >= GViewport.bottom;}
static bool insideRight(const Point& p) {return p.x < GViewport.right;}
static bool insideTop(const Point& p) {return p.y < GViewport.top;}

void Polygon::clip(const Rect& view) {
    vector<Point> v;
    bool (*(insideFunc[]))(const Point& p) = {insideLeft, insideBottom, insideRight, insideTop};
    Line lines[] = {Line(1, 0, -view.left), Line(0, 1, -view.bottom),
        Line(1, 0, -view.right+1), Line(0, 1, -view.top+1)};
    for (int k = 0; k < 4; k++) {
        v.clear();
        for (int i = 0; i < list_.size(); i++)
            cout << '(' << list_[i] << ')';
        cout << endl;
        for (int i = 0; i < list_.size(); i++) {
            int pi = (i + list_.size() - 1) % list_.size();
            if (insideFunc[k](list_[i]) ^ insideFunc[k](list_[pi])) {
                Line l(list_[i], list_[pi]);
                Point p;
                if (!intersection(l, lines[k], p))
                    v.push_back(p);
            }
            if (insideFunc[k](list_[i]))
                v.push_back(list_[i]);
        }
        list_ = v;
    }

        for (int i = 0; i < list_.size(); i++)
            cout << '(' << list_[i] << ')';
        cout << endl;
}

const Point& Polygon::centroid() {
    if (!validCentroid)
        genCentroid();
    return centroid_;
}

const vector<Point>& Polygon::list() const {
    return list_;
}
