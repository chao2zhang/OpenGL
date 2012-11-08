#include "dataManager.h"
#include "gl.h"
#include <fstream>
#include <iomanip>
#include <set>
#include <algorithm>

using namespace std;

void DataManager::load(const char filename[],
                     vector<Point>& points,
                     vector<Triangle>& triangles,
                     int& n) {
    points.clear();
    triangles.clear();
    ifstream in(filename);
    if (!in)
        return;
    in >> n;
    for (int i = 0; i < n; i++) {
        int v;
        in >> v;
        for (int j = 0; j < v; j++) {
            Point p;
            in >> p.point >> p.color;
            p.id = i;
            points.push_back(p);
        }
        int t;
        in >> t;
        for (int j = 0; j < t; j++) {
            Triangle t;
            in >> t.v;
            t.id = i;
            triangles.push_back(t);
        }
    }
    in.close();
}

void DataManager::dump(const char filename[],
                     const vector<Point>& points,
                     const vector<Triangle>& triangles,
                     int n) {
    ofstream out(filename);
    if (!out)
        return;
    out << n << endl << endl;
    out << setprecision(3) << setiosflags(ios::fixed);
    for (int i = 0; i < n; i++) {
        int size = 0;
        for (int j = 0; j < points.size(); j++)
            size += points[j].id == i;
        out << size << endl;
        for (int j = 0; j < points.size(); j++)
            if (points[j].id == i)
                out << points[j].point << ' ' << points[j].color << endl;
        out << endl;
        size = 0;
        for (int j = 0; j < triangles.size(); j++)
            size += triangles[j].id == i;
        out << size << endl;
        for (int j = 0; j < triangles.size(); j++)
            if (triangles[j].id == i)
                out << triangles[j].v << endl;
    }
    out.close();
}
