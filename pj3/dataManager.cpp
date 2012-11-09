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
    int v;
    in >> v;
    for (int j = 0; j < v; j++) {
        Point p;
        in >> p.point >> p.color;
        points.push_back(p);
    }
    int t;
    in >> t;
    for (int j = 0; j < t; j++) {
        Triangle t;
        in >> t.v;
        triangles.push_back(t);
    }
    in >> n;
    for (int j = 0; j < n; j++) {
        int c;
        in >> c;
        while(c--) {
            int p;
            in >> p;
            points[p].id = j;
        }
    }
    for (int j = 0; j < t; j++) {
        triangles[j].id = points[triangles[j].v.x].id;
    }
    in.close();
}

void DataManager::save(const char filename[],
                     const vector<Point>& points,
                     const vector<Triangle>& triangles,
                     int n) {
    ofstream out(filename);
    if (!out)
        return;
    out << setprecision(3) << setiosflags(ios::fixed);
    out << points.size() << endl;
    for (int j = 0; j < points.size(); j++)
        out << points[j].point << ' ' << points[j].color << endl;
    out << endl;
    out << triangles.size() << endl;
    for (int j = 0; j < triangles.size(); j++)
        out << triangles[j].v << endl;
    out << endl;
    out << n << endl;
    for (int i = 0; i < n; i++) {
        int count = 0;
        for (int j = 0; j < points.size(); j++)
            count += points[j].id == i;
        out << count << ' ';
        for (int j = 0; j < points.size(); j++)
            if (points[j].id == i) out << j << ' ';
        out << endl;
    }
    out.close();
}
