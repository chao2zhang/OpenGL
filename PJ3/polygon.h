#ifndef POLYGON_H
#define POLYGON_H

#include <GL/glut.h>
#include <vector>
#include <utility>
#include <string>
#include "point.h"
#include "line.h"
#include "matrix.h"

typedef std::vector<Point3f>::iterator ItVertex;
typedef std::pair<int, int> Edge;

class Polygon {
private:
    std::vector<Point3f> _vertices;
    std::vector<Edge> _edges;
    Point3f _centroid;
    bool validCentroid;
    void genCentroid();
    int _id;
public:
    Polygon():validCentroid(false), _id(0){}
    Polygon(int id):validCentroid(false), _id(id){}
    void addVertex3f(const Point3f& v);
    void addEdge2i(const std::pair<int, int>& e);
    void project2D(ProjType p, const Point2i& view) const;
    void projectCabinet(const Point2i& view) const;
    void projectCavalier(const Point2i& view) const;
    void show() const;
    void translate(const Point3f& p);
    void rotate(const Point3f& p1, const Point3f& p2, float a);
    void scale(float a);
    void scale(const Point3f& p, float a);
    const Point3f& centroid();
    const std::vector<Point3f>& vertices() const;
    const std::vector<Edge>& edges() const;
    const int id() const;
    std::string toString() const;
};

#endif
