#include "polygon.h"
#include "utils.h"
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

void Polygon::addVertex3f(const Point3f& p) {
    _vertices.push_back(p);
}

void Polygon::addEdge2i(const Edge& e) {
    _edges.push_back(e);
}

void Polygon::genCentroid() {
    if (_vertices.size() == 0)
        return;
    Point3f sum;
    for (int i = 0; i < _vertices.size(); i++) {
        sum += _vertices[i];
    }
    _centroid = sum / _vertices.size();
    validCentroid = true;
}

void Polygon::project2D(ProjType p, const Point2i& view) const{
    glBegin(GL_LINES);
    for (int i = 0; i < _edges.size(); i++) {
        _vertices[_edges[i].first - 1].project2D(p, view);
        _vertices[_edges[i].second - 1].project2D(p, view);
    }
    glEnd();
    glBegin(GL_POINTS);
    for (int i = 0; i < _vertices.size(); i++)
        _vertices[i].project2D(p, view);
    glEnd();
}

void Polygon::projectCabinet(const Point2i& view) const{
    glBegin(GL_LINES);
    for (int i = 0; i < _edges.size(); i++) {
        _vertices[_edges[i].first - 1].projectCabinet(view);
        _vertices[_edges[i].second - 1].projectCabinet(view);
    }
    glEnd();
    glBegin(GL_POINTS);
    for (int i = 0; i < _vertices.size(); i++)
        _vertices[i].projectCabinet(view);
    glEnd();
}

void Polygon::projectCavalier(const Point2i& view) const {
    glBegin(GL_LINES);
    for (int i = 0; i < _edges.size(); i++) {
        _vertices[_edges[i].first - 1].projectCavalier(view);
        _vertices[_edges[i].second - 1].projectCavalier(view);
    }
    glEnd();
    glBegin(GL_POINTS);
    for (int i = 0; i < _vertices.size(); i++)
        _vertices[i].projectCavalier(view);
    glEnd();
}

void Polygon::show() const {
    glBegin(GL_LINES);
    for (int i = 0; i < _edges.size(); i++) {
        _vertices[_edges[i].first - 1].show();
        _vertices[_edges[i].second - 1].show();
    }
    glEnd();
    glBegin(GL_POINTS);
    for (int i = 0; i < _vertices.size(); i++)
        _vertices[i].show();
    glEnd();
}

void Polygon::translate(const Point3f& p) {
    for (int i = 0; i < _vertices.size(); i++) {
        _vertices[i] += p;
    }
}

void Polygon::rotate(const Point3f& p1, const Point3f& p2, float a) {
    Point3f u = p2 - p1;
    u /= u.length();
    float cosa = cos(a / 180 * PI);
    float sina = sin(a / 180 * PI);
    float onec = 1 - cosa;
    Matrix rot(3, 3);
    rot[0][0] = u.x * u.x * onec + cosa;
    rot[0][1] = u.x * u.y * onec - u.z * sina;
    rot[0][2] = u.x * u.z * onec + u.y * sina;
    rot[1][0] = u.y * u.x * onec + u.z * sina;
    rot[1][1] = u.y * u.y * onec + cosa;
    rot[1][2] = u.z * u.y * onec + u.x * sina;
    rot[2][0] = u.x * u.z * onec + u.y * sina;
    rot[2][1] = u.y * u.z * onec - u.x * sina;
    rot[2][2] = u.z * u.z * onec + cosa;
    for (int i = 0; i < _vertices.size(); i++) {
        Point3f p = _vertices[i] - p1;
        Matrix v(3, 1);
        v[0][0] = p.x; v[1][0] = p.y; v[2][0] = p.z;
        Matrix r = rot * v;
        p.x = r[0][0]; p.y = r[1][0]; p.z = r[2][0];
        _vertices[i] = p + p1;
    }
}

void Polygon::scale(float a) {
    genCentroid();
    for (int i = 0; i < _vertices.size(); i++) {
        _vertices[i] -= _centroid;
        _vertices[i] *= a;
        _vertices[i] += _centroid;
    }
}

void Polygon::scale(const Point3f& p, float a) {
    for (int i = 0; i < _vertices.size(); i++) {
        _vertices[i] -= p;
        _vertices[i] *= a;
        _vertices[i] += p;
    }
}

const Point3f& Polygon::centroid() {
    if (!validCentroid)
        genCentroid();
    return _centroid;
}

const vector<Point3f>& Polygon::vertices() const {
    return _vertices;
}

const vector<Edge>& Polygon::edges() const {
    return _edges;
}

const int Polygon::id() const {
    return _id;
}

string Polygon::toString() const {
    string ret;
    ret += format("Object #%d\n", _id);
    ret += format("Vertices #%d:\n", _vertices.size());
    for (int i = 0; i < _vertices.size(); i++)
        ret += format("\t#%d:(%.3f, %.3f, %.3f)", i + 1, _vertices[i].x, _vertices[i].y, _vertices[i].z);
    ret += '\n';
    ret += format("Edges #%d:\n", _edges.size());
    for (int i = 0; i < _edges.size(); i++)
        ret += format("\t(%d, %d)", _edges[i].first, _edges[i].second);
    ret += '\n';
    return ret;
}
