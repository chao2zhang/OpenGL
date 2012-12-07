#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <ctime>
#include <iostream>
#include <sstream>
#include <vector>
#include "gl.h"
#include "object.h"

using namespace std;

#define WINDOW_SIZE 600

const Point2i Window(WINDOW_SIZE, WINDOW_SIZE);
int window;

float canvas[WINDOW_SIZE * WINDOW_SIZE * 3];

vector<Primitive*> objects;

Point3f from(0, -3, 0);
Point3f at(0, 0, 0);
Point3f up(0, 0, 1);
Point3f rv;
float angDeg = 45;
int resolution = WINDOW_SIZE;
int depth = 5;

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
            Color3f f = light(r, objects, depth);
            makePixel(x, y, f, canvas, resolution);
        }
    normalize(canvas, resolution);
    glDrawPixels(resolution, resolution, GL_RGB, GL_FLOAT, canvas);
    glutSwapBuffers();
}

void refreshFunc() {
    glutPostWindowRedisplay(window);
}

void get() {
    cout << "from=" << from << endl;
    cout << "at=" << at << endl;
    cout << "up=" << up << endl;
    cout << "angle=" << angDeg << endl;
    cout << "I_a=" << iA << " I_l=" << iL << endl;
    cout << "k_a=" << kA << " k_d=" << kD << " k_s=" << kS << endl;
    cout << "K=" << bigK << " n=" << bigN << endl;
    cout << "k_r=" << kR << " k_t=" << kT << endl;
    cout << "eta=" << eta << endl;
    cout << "depth=" << depth << endl;
    cout << "resolution=" << resolution << endl;
}

void set(const string& command) {
    if (!command.size()) return;
    int pos = command.find('=');
    string name = command.substr(0, pos);
    stringstream value(command.substr(pos + 1, command.size() - pos - 1));
    cout << name << " " << value.str() << endl;
    if (name == "from") value >> from;
    if (name == "at") value >> at;
    if (name == "up") value >> up;
    if (name == "angle") value >> angDeg;
    if (name == "I_a") value >> iA;
    if (name == "I_l") value >> iL;
    if (name == "k_a") value >> kA;
    if (name == "k_d") value >> kD;
    if (name == "k_s") value >> kS;
    if (name == "K") value >> bigK;
    if (name == "n") value >> bigN;
    if (name == "k_r") value >> kR;
    if (name == "k_t") value >> kT;
    if (name == "eta") value >> eta;
    if (name == "depth") value >> depth;
    if (name == "resolution") value >> resolution;
}

void set() {
    string command;
    cout << ">";
    getline(cin, command);
    set(command);
    updateVector();
    cout << "OK!" << endl;
    refreshFunc();
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
    Quadratic* s2 = newSphere(Point3f(1, 1.5, 1), 0.6);
    s2->color = Color3f(1, 1, 1);
    s2->transparent = true;
    Polygon* t = newTriangle(Point3f(-1, 2, 2),
                             Point3f(-1, 2, 1),
                             Point3f(-1, 1, 2),
                             Point3f(-1, 0, 0));
    t->color = Color3f(1, 1, 1);
    t->transparent = true;
    Polygon* t2 = newTriangle(Point3f(0, 2, 2),
                              Point3f(-1, 2, 1),
                              Point3f(-1, 2, 2),
                              Point3f(0, 1, 0));
    t2->color = Color3f(1, 1, 1);
    t2->transparent = true;
    Polygon* t3 = newTriangle(Point3f(-1, 2, 2),
                              Point3f(-1, 1, 2),
                              Point3f(0, 2, 2),
                              Point3f(0, 0, 1));
    t3->color = Color3f(1, 1, 1);
    t3->transparent = true;
    Polygon* t4 = newTriangle(Point3f(0, 2, 2),
                              Point3f(-1, 1, 2),
                              Point3f(-1, 2, 1),
                              Point3f(1, -1, -1));
    t4->color = Color3f(1, 1, 1);
    t4->transparent = true;
    Polygon* p = newSquare(Point3f(5, 6, -4), Point3f(5, 6, 4),
                           Point3f(-5, 6, 4), Point3f(-5, 6, -4),
                           Point3f(0, -1, 0));
    p->color = Color3f(1, 0.85, 0.25);
    p->transparent = false;
    Polygon* p2 = newSquare(Point3f(-5, -6, -4), Point3f(-5, 6, -4),
                           Point3f(-5, 6, 4), Point3f(-5, -6, 4),
                           Point3f(1, 0, 0));
    p2->color = Color3f(0, 0.5, 1);
    p2->transparent = false;
    Polygon* p3 = newSquare(Point3f(5, 6, -4), Point3f(5, -6, -4),
                           Point3f(5, -6, 4), Point3f(5, 6, 4),
                           Point3f(-1, 0, 0));
    p3->color = Color3f(1, 0.2, 0);
    p3->transparent = false;
    Polygon* p4 = newSquare(Point3f(5, 6, 4), Point3f(5, -6, 4),
                           Point3f(-5, -6, 4), Point3f(-5, 6, 4),
                           Point3f(0, 0, -1));
    p4->color = Color3f(0.5, 1, 0);
    p4->transparent = false;
    objects.push_back(s);
    objects.push_back(s2);
    objects.push_back(t);
    objects.push_back(t2);
    objects.push_back(t3);
    objects.push_back(t4);
    objects.push_back(p);
    objects.push_back(p2);
    objects.push_back(p3);
    objects.push_back(p4);
    updateVector();
}

void keyFunc(unsigned char ch, int x, int y) {
    if (ch == 'p')
        get();
    if (ch == 's')
        set();
}

int main(int argc, char** argv) {
    init(argc, argv);
    window = glutCreateWindow("OpenGL Project 5");
    initWindow();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyFunc);
    atexit(exitFunc);
    glutMainLoop();
    return 0;
}
