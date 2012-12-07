#include "object.h"
#include "base.h"
#include <iostream>
#include <string>

float iA = 0.2, iL = 1;
Point3f kA(1, 1, 1), kD(1, 1, 1), kS(1, 1, 1);
int bigN = 5;
float bigK = 10;
float kR = 0.5, kT = 0.5;
float eta = 1.5;
Point3f lightSource(0, 1, 0);

using namespace std;

bool Polygon::intersect(const Line& l, Point3f& result) {
    float D = -dot(n, v[0]);
    float s = -(D+dot(n, l.start)) / dot(n, l.u);
    bool ret = POS(s);
    result = l.start + l.u * s;
    for (int i = 0; i < v.size(); i++)
        if (ret)
            ret = ret && dot(cross(v[i % v.size()] - result, v[(i+1) % v.size()] - result), n) > 0;
    return ret;
}

Point3f Polygon::normal(const Point3f& p) {
    return n;
}

bool Sphere::intersect(const Line& l, Point3f& result) {
    Point3f diff = center - l.start;
    float base = dot(l.u, diff);
    float delta = base * base - dot(diff, diff) + radius * radius;
    if (delta < 0)
        return false;
    delta = sqrt(delta);
    Point3f p1 = l.start + (base - delta) * l.u;
    Point3f p2 = l.start + (base + delta) * l.u;
    bool v1 = POS((p1 - l.start).length()) && POS(base - delta);
    bool v2 = POS((p2 - l.start).length()) && POS(base + delta);
    if (v1 && v2)
        result = (p1 - l.start).length() < (p2 - l.start).length() ? p1 : p2;
    else if (v1)
        result = p1;
    else if (v2)
        result = p2;
    else return false;
    return true;
}

Point3f Sphere::normal(const Point3f& p) {
    Point3f ret = (p - center).norm();
    return ret;
}

bool Quadratic::intersect(const Line& l, Point3f& result) {
    float Aq = A * l.u.x * l.u.x + B * l.u.y * l.u.y + C * l.u.z * l.u.z
                + D * l.u.x * l.u.y + E * l.u.x * l.u.z + F * l.u.y * l.u.z;
    float Bq = 2*A * l.start.x * l.u.x + 2*B * l.start.y * l.u.y + 2*C * l.start.z * l.u.z
                + D * (l.start.x * l.u.y + l.start.y * l.u.x)
                + E * (l.start.x * l.u.z + l.start.z * l.u.x)
                + F * (l.start.y * l.u.z + l.start.z * l.u.y)
                + G * l.u.x + H * l.u.y + I * l.u.z;
    float Cq = A * l.start.x * l.start.x + B * l.start.y * l.start.y + C * l.start.z * l.start.z
                + D * l.start.x * l.start.y + E * l.start.x * l.start.z + F * l.start.y * l.start.z
                + G * l.start.x + H * l.start.y + I * l.start.z + J;
    float delta = Bq * Bq - 4 * Aq * Cq;
    if (delta < 0)
        return false;
    delta = sqrt(delta);
    Point3f p1, p2;
    if (Aq == 0)
        p1 = p2 = l.start + (-Cq / Bq) * l.u;
    else {
        p1 = l.start + (- Bq - delta) / 2 / Aq * l.u;
        p2 = l.start + (- Bq + delta) / 2 / Aq * l.u;
    }
    bool v1 = POS((p1 - l.start).length()) && POS(- Bq - delta);
    bool v2 = POS((p2 - l.start).length()) && POS(- Bq + delta);
    if (v1 && v2)
        result = (p1 - l.start).length() < (p2 - l.start).length() ? p1 : p2;
    else if (v1)
        result = p1;
    else if (v2)
        result = p2;
    else return false;
    return true;
}

Point3f Quadratic::normal(const Point3f& p) {
    Point3f ret;
    ret.x = 2*A*p.x + D*p.y + E*p.z + G;
    ret.y = 2*B*p.y + D*p.x + F*p.z + H;
    ret.z = 2*C*p.z + E*p.x + F*p.y + I;
    ret.norm();
    return ret;
}

Polygon* newTriangle(const Point3f& v1, const Point3f& v2, const Point3f& v3, const Point3f& normal) {
    Polygon* ret = new Polygon();
    ret->v.push_back(v1);
    ret->v.push_back(v2);
    ret->v.push_back(v3);
    ret->n = normal;
    return ret;
}

Polygon* newSquare(const Point3f& v1, const Point3f& v2, const Point3f& v3, const Point3f& v4, const Point3f& normal) {
    Polygon* ret = new Polygon();
    ret->v.push_back(v1);
    ret->v.push_back(v2);
    ret->v.push_back(v3);
    ret->v.push_back(v4);
    ret->n = normal;
    return ret;
}

Quadratic* newSphere(const Point3f& center, float radius) {
    Quadratic* ret = new Quadratic();
    ret->A = ret->B = ret->C = 1;
    ret->D = ret->E = ret->F = 0;
    ret->G = - 2 * center.x;
    ret->H = - 2 * center.y;
    ret->I = - 2 * center.z;
    ret->J = dot(center, center) - radius * radius;
    return ret;
}

Quadratic* newEllipsoid(const Point3f& center, float a, float b, float c) {
    Quadratic* ret = new Quadratic();
    ret->A = b * b + c * c;
    ret->B = a * a + c * c;
    ret->C = a * a + b * b;
    ret->D = ret->E = ret->F = 0;
    ret->G = b * b * c * c * -2 * center.x;
    ret->H = a * a * c * c * -2 * center.y;
    ret->I = a * a * b * b * -2 * center.z;
    ret->J = b * b * c * c * center.x * center.x
        +   a * a * c * c * center.y * center.y
        +   a * a * b * b * center.z * center.z
        -   a * a * b * b * c * c;
    return ret;
}

Point3f refract(const Line& l, const Point3f& normal) {
    float r = l.inside ? eta : 1 / eta;
    float cosThetaI = dot(-1 * l.u, normal);
    float cosThetaR = sqrt(1 - (r * r) * (1 - cosThetaI * cosThetaI));
    return r * l.u - (cosThetaR - r * cosThetaI) * normal;
}

Point3f reflect(const Point3f& u, const Point3f& normal) {
    return u - 2 * normal * dot(normal, u);
}

Point3f phong(const Point3f& point, const Point3f& ref, const Point3f& normal, const vector<Primitive*>& primitives) {
    Point3f intensity = kA * iA;
    Point3f l = (lightSource - point).norm();
    Point3f v = (ref - point).norm();
    Point3f r = (2 * normal * dot(normal, l) - l).norm();
    Point3f diff;
    Line line; line.start = point; line.u = (lightSource - point).norm();
    Point3f result;
    int resultId;
    if (intersect(line, primitives, result, resultId))
        return intensity;
    float productDiff = dot(l, normal);
    if (productDiff > 0) diff = productDiff * kD;
    Point3f spec;
    float productSpec = dot(r, v);
    if (productDiff > 0 && productSpec > 0) spec = productSpec * kS;
    intensity += iL / ((ref - point).length() + bigK) * (diff + spec);
    return intensity;
}

bool intersect(const Line& line, const vector<Primitive*>& primitives, Point3f& result, int& resultId) {
    Point3f best, now;
    int bestId;
    float bestDist = 0, nowDist;
    bool ret = false;
    for (int i = 0; i < primitives.size(); i++) {
        if (primitives[i]->intersect(line, now)) {
            nowDist = (now - line.start).length();
            if (!ret || (nowDist < bestDist)) {
                ret = true;
                best = now;
                bestId = i;
                bestDist = nowDist;
            }
        }
    }
    if (ret) {
        result = best;
        resultId = bestId;
    }
    return ret;
}

Color3f light(const Line& line, const vector<Primitive*>& primitives, int depth) {
    Color3f ret;
    Primitive* prim;
    int id;
    Point3f point;
    Point3f normal;
    if (depth == 0) return ret;
    if (intersect(line, primitives, point, id)) {
        prim = primitives[id];
        normal = prim->normal(point);
        Point3f ph = phong(point, line.start, normal, primitives);
        ret = prim->color * ph;
        Line refl;
        refl.start = point;
        refl.u = reflect(line.u, normal).norm();
        refl.inside = line.inside;
        Color3f rf = light(refl, primitives, depth - 1);
        ret += rf * kR;
        if (prim->transparent) {
            Line refr;
            refr.start = point;
            refr.u = refract(line, normal).norm();
            refr.inside = !line.inside;
            Color3f rt = light(refr, primitives, depth - 1);
            ret += rt * kT;
        }
    }
    return ret;
}

