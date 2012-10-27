#include "polygon.h"
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
    float sx = 0, sy = 0, sz = 0;
    for (int i = 0; i < _vertices.size(); i++) {
        sx += _vertices[i].x;
        sy += _vertices[i].y;
        sz += _vertices[i].z;
    }
    _centroid.x = sx / _vertices.size();
    _centroid.y = sy / _vertices.size();
    _centroid.z = sz / _vertices.size();
    validCentroid = true;
}

void Polygon::project2D(ProjType p, const Point2i& view) const{
    glBegin(GL_LINES);
    for (int i = 0; i < _edges.size(); i++) {
        const Point3f& p1 = _vertices[_edges[i].first - 1];
        const Point3f& p2 = _vertices[_edges[i].second - 1];
        switch(p) {
            case PROJ_XY:
                glVertex2f(p1.x * view.x, p1.y * view.y);
                glVertex2f(p2.x * view.x, p2.y * view.y);
                break;
            case PROJ_YZ:
                glVertex2f(p1.y * view.x, p1.z * view.y);
                glVertex2f(p2.y * view.x, p2.z * view.y);
                break;
            case PROJ_XZ:
                glVertex2f(p1.x * view.x, p1.z * view.y);
                glVertex2f(p2.x * view.x, p2.z * view.y);
                break;
            default:
                break;
        }
    }
    glEnd();
}

void Polygon::projectCabinet(const Point2i& view) const{
    glBegin(GL_LINES);
     for (int i = 0; i < _edges.size(); i++) {
        const Point3f& p1 = _vertices[_edges[i].first - 1];
        const Point3f& p2 = _vertices[_edges[i].second - 1];
        glVertex2f(
            (p1.x - CABINET_INV * p1.z) * view.x,
            (p1.y - CABINET_INV * p1.z) * view.y
        );
        glVertex2f(
            (p2.x - CABINET_INV * p2.z) * view.x,
            (p2.y - CABINET_INV * p2.z) * view.y
        );
    }
    glEnd();
}

void Polygon::projectCavalier(const Point2i& view) const {
    glBegin(GL_LINES);
     for (int i = 0; i < _edges.size(); i++) {
        const Point3f& p1 = _vertices[_edges[i].first - 1];
        const Point3f& p2 = _vertices[_edges[i].second - 1];
        glVertex2f(
            (p1.x - CAVALIER_INV * p1.z) * view.x,
            (p1.y - CAVALIER_INV * p1.z) * view.y
        );
        glVertex2f(
            (p2.x - CAVALIER_INV * p2.z) * view.x,
            (p2.y - CAVALIER_INV * p2.z) * view.y
        );
    }
    glEnd();
}

void Polygon::show() const {
    glBegin(GL_LINES);
     for (int i = 0; i < _edges.size(); i++) {
        const Point3f& p1 = _vertices[_edges[i].first - 1];
        const Point3f& p2 = _vertices[_edges[i].second - 1];
        glVertex3f(p1.x, p1.y, p1.z);
        glVertex3f(p2.x, p2.y, p2.z);
    }
    glEnd();
}

void Polygon::translate(const Point3f& p) {
    for (int i = 0; i < _vertices.size(); i++) {
        _vertices[i].x += p.x;
        _vertices[i].y += p.y;
        _vertices[i].z += p.z;
    }

}

void Polygon::scale(float a) {
    genCentroid();
    for (int i = 0; i < _vertices.size(); i++) {
        Point3f p = _vertices[i];
        _vertices[i].x = (p.x - _centroid.x) * a + _centroid.x;
        _vertices[i].y = (p.y - _centroid.y) * a + _centroid.y;
        _vertices[i].z = (p.z - _centroid.z) * a + _centroid.z;
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
