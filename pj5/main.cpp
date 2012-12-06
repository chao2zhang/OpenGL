#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <ctime>
#include <iostream>
#include <vector>
#include "gl.h"
#include "object.h"

using namespace std;

#define WINDOW_SIZE 600

const Point2i Window(WINDOW_SIZE, WINDOW_SIZE);
int window;

float canvas[WINDOW_SIZE * WINDOW_SIZE * 3];

vector<Primitive*> objects;

Point3f from(0, -1, 0);
Point3f at(0, 0, 0);
float angDeg = 45;
Point3f up(0, 0, 1);
Point3f rv;
int resolution = WINDOW_SIZE;

Line ray(int x, int y) {
    Line ret;
    ret.start = from;
    float xx = (float)x/(float)resolution*2 - 1;
    float yy = (float)y/(float)resolution*2 - 1;
    Point3f p = from;
    p += tan(angDeg / 180 * PI) * xx * rv;
    p += tan(angDeg / 180 * PI) * yy * up;
    p += (at - from).norm();
    ret.u = (p - from).norm();
    return ret;
}

void updateVector() {
    up.norm();
    rv = cross(at - from, up).norm();
}

void initWindow() {
    glClearColor(0, 0, 0, 0);
}

void call() {
    1;
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    memset(canvas, 0.0f, sizeof(canvas));
    for (int x = 0; x < resolution; x++)
        for (int y = 0; y < resolution; y++) {
            Line r = ray(x, y);
            Color3f f = light(r, objects, 5);
            makePixel(x, y, f, canvas, resolution);
        }
    normalize(canvas, resolution);
    glDrawPixels(resolution, resolution, GL_RGB, GL_FLOAT, canvas);
    glutSwapBuffers();
}

void refreshFunc() {
    glutPostWindowRedisplay(window);
}

void exitFunc() {
}

void init(int argc, char** argv) {
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(Window.x, Window.y);
    glutInitWindowPosition(100, 100);

    Quadratic* s = newSphere(Point3f(-0.5, 1, -0.5), 0.4);
    s->color = Color3f(1, 1, 1);
    s->transparent = false;
    Quadratic* s2 = newSphere(Point3f(0.5, 1, 0.5), 0.6);
    s2->color = Color3f(1, 1, 1);
    s2->transparent = true;
    Polygon* p = newSquare(Point3f(5, 10, -4), Point3f(5, 10, 4),
                           Point3f(-5, 10, 4), Point3f(-5, 10, -4),
                           Point3f(0, -1, 0));
    p->color = Color3f(0.8, 0.8, 0.2);
    p->transparent = false;
    Polygon* p2 = newSquare(Point3f(-5, -10, -4), Point3f(-5, 10, -4),
                           Point3f(-5, 10, 4), Point3f(-5, -10, 4),
                           Point3f(1, 0, 0));
    p2->color = Color3f(0.2, 0.8, 0.8);
    p2->transparent = false;
    Polygon* p3 = newSquare(Point3f(5, 10, -4), Point3f(5, -10, -4),
                           Point3f(5, -10, 4), Point3f(5, 10, 4),
                           Point3f(-1, 0, 0));
    p3->color = Color3f(0.8, 0.2, 0.8);
    p3->transparent = false;
    objects.push_back(s);
    objects.push_back(s2);
    objects.push_back(p);
    objects.push_back(p2);
    objects.push_back(p3);
    updateVector();
}

int main(int argc, char** argv) {
    init(argc, argv);
    window = glutCreateWindow("OpenGL Project 3");
    initWindow();
    glutDisplayFunc(display);
    atexit(exitFunc);
    glutMainLoop();
    return 0;
}
