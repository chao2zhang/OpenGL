#ifndef POLYGON_H
#define POLYGON_H

#include "gl.h"
#include <vector>
#include <utility>
#include <string>

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
    void scale(float a);
    const Point3f& centroid();
    const std::vector<Point3f>& vertices() const;
    const std::vector<Edge>& edges() const;
    const int id() const;
    std::string toString() const;
};

#endif
