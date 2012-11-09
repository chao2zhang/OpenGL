#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <ctime>
#include <iostream>
#include <vector>
#include "gl.h"
#include "object.h"
#include "dataManager.h"
#include "console.h"

using namespace std;

#define WINDOW_SIZE 600

const Point2i Window(WINDOW_SIZE, WINDOW_SIZE);
const Point2i ConsoleWindow(580, 580);
const Point2i TextOffset(0, 5);

int mainWindow;
int xyWindow, yzWindow, xzWindow;
int consoleWindow;

float xyCanvas[WINDOW_SIZE * WINDOW_SIZE * 3];
float yzCanvas[WINDOW_SIZE * WINDOW_SIZE * 3];
float xzCanvas[WINDOW_SIZE * WINDOW_SIZE * 3];

Point3f *rotPoint1, *rotPoint2;

string file;
DataManager manager;

vector<Point> points;
vector<Triangle> triangles;
int n;

void initMainWindow() {
    glClearColor(0.8, 0.8, 0.8, 0);
    gluOrtho2D(0, Window.x, 0, Window.y);
}

void initSubWindow() {
    glClearColor(1, 1, 1, 0);
}

void initConsoleWindow() {
    glClearColor(0, 0, 0, 0);
    gluOrtho2D(0, ConsoleWindow.x, 0, ConsoleWindow.y);
}

void displayMain() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3b(0, 0, 0);
    glutSwapBuffers();
}

#define KA 0.2
#define KD 1.5
#define KS 3.0
#define SP 2
const Color3f IA(1.0, 1.0, 1.0);
const Color3f IL(1.0, 1.0, 1.0);

void displayXY() {
    cout << ">>>>BEGIN XY-Plane" << endl;
    glClear(GL_COLOR_BUFFER_BIT);
    normal(points, triangles, n);
    projectOrtho(points, PLANE_XY);
    Point3f viewPoint(0.5, 0.5, -10);//D
    Point3f viewVector(0, 0, -1);
    Point3f lightSource(0.5, 0.5, 0);
    sort(points, triangles, viewPoint, viewVector);
    light(points, KA, KD, KS, IA, IL, lightSource, viewPoint, SP);
    triangle(points, triangles, xyCanvas, WINDOW_SIZE);
    glDrawPixels(WINDOW_SIZE, WINDOW_SIZE, GL_RGB, GL_FLOAT, xyCanvas);
    glutSwapBuffers();
    for (int i = 0; i < points.size(); ++i)
        cout << points[i].toString() << endl;
    for (int i = 0; i < triangles.size(); ++i)
        cout << triangles[i].v << endl;
    cout << "<<<<END XY-Plane" << endl;
}

void displayYZ() {
    cout << ">>>>BEGIN YZ-Plane" << endl;
    glClear(GL_COLOR_BUFFER_BIT);
    normal(points, triangles, n);
    projectOrtho(points, PLANE_YZ);
    Point3f viewPoint(10, 0.5, 0.5);//Right to left
    Point3f viewVector(1, 0, 0);
    Point3f lightSource(0.5, 0.5, 0);
    sort(points, triangles, viewPoint, viewVector);
    light(points, KA, KD, KS, IA, IL, lightSource, viewPoint, SP);
    triangle(points, triangles, yzCanvas, WINDOW_SIZE);
    glDrawPixels(WINDOW_SIZE, WINDOW_SIZE, GL_RGB, GL_FLOAT, yzCanvas);
    glutSwapBuffers();
    for (int i = 0; i < points.size(); ++i)
        cout << points[i].toString() << endl;
    for (int i = 0; i < triangles.size(); ++i)
        cout << triangles[i].v << endl;
    cout << "<<<<END YZ-Plane" << endl;
}


void displayXZ() {
    cout << ">>>>BEGIN XZ-Plane" << endl;
    glClear(GL_COLOR_BUFFER_BIT);
    normal(points, triangles, n);
    projectOrtho(points, PLANE_XZ);
    Point3f viewPoint(0.5, -10, 0.5);//D
    Point3f viewVector(0, -1, 0);
    Point3f lightSource(0.5, 0.5, 0);
    sort(points, triangles, viewPoint, viewVector);
    light(points, KA, KD, KS, IA, IL, lightSource, viewPoint, SP);
    triangle(points, triangles, xzCanvas, WINDOW_SIZE);
    glDrawPixels(WINDOW_SIZE, WINDOW_SIZE, GL_RGB, GL_FLOAT, xzCanvas);
    glutSwapBuffers();
    for (int i = 0; i < points.size(); ++i)
        cout << points[i].toString() << endl;
    for (int i = 0; i < triangles.size(); ++i)
        cout << triangles[i].v << endl;
    cout << "<<<<END XZ-Plane" << endl;
}

void displayConsole() {
    glClear(GL_COLOR_BUFFER_BIT);
    Point2i p(0, ConsoleWindow.y - 13);
    print(GLUT_BITMAP_8_BY_13, hint, p, ConsoleWindow);
    print(GLUT_BITMAP_8_BY_13, hintInput, p, ConsoleWindow);
    print(GLUT_BITMAP_8_BY_13, currInput, p, ConsoleWindow);
    print(GLUT_BITMAP_8_BY_13, "_", p, ConsoleWindow);
    glutSwapBuffers();
}

void refreshFunc() {
    glutPostWindowRedisplay(mainWindow);
    glutPostWindowRedisplay(xyWindow);
    glutPostWindowRedisplay(yzWindow);
    glutPostWindowRedisplay(xzWindow);
    glutPostWindowRedisplay(consoleWindow);
}

void keyFunc(unsigned char ch, int x, int y) {
    inputCharacter(ch);
    refreshFunc();
}

void exitFunc() {
}

void clearRotPoints() {
    if (rotPoint1) delete rotPoint1;
    if (rotPoint2) delete rotPoint2;
    rotPoint1 = rotPoint2 = NULL;
}

void translate(const Point3f& p) {
    clearRotPoints();
}

void rotate1(const Point3f& p1) {
    rotPoint1 = new Point3f(p1);
}

void rotate2(const Point3f& p1, const Point3f& p2) {
    rotPoint2 = new Point3f(p2);
}

void rotate(const Point3f& p1, const Point3f& p2, float a) {
}

void scale(float a) {
    clearRotPoints();
}

void load(const std::string& filename) {
    file = filename;
    manager.load(file.c_str(), points, triangles, n);
}

void save() {
    manager.save(file.c_str(), points, triangles, n);
}

void normalize() {
}

void init(int argc, char** argv) {
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(Window.x, Window.y);
    glutInitWindowPosition(100, 100);

    nextCallback = NULL;
    prevCallback = NULL;
    translateCallback = translate;
    rotateCallback = rotate;
    rotate1Callback = rotate1;
    rotate2Callback = rotate2;
    scaleCallback = scale;
    loadCallback = load;
    saveCallback = save;
    normalizeCallback = normalize;
}

int main(int argc, char** argv) {

    init(argc, argv);

    mainWindow = glutCreateWindow("OpenGL Project 3");
    initMainWindow();
    glutDisplayFunc(displayMain);
    glutKeyboardFunc(keyFunc);

    xyWindow = glutCreateWindow("XY-PLANE");
    initSubWindow();
    glutPositionWindow(200, 100);
    glutDisplayFunc(displayXY);

    yzWindow = glutCreateWindow("YZ-PLANE");
    initSubWindow();
    glutPositionWindow(300, 100);
    glutDisplayFunc(displayYZ);

    xzWindow = glutCreateWindow("XZ-PLANE");
    initSubWindow();
    glutPositionWindow(400, 100);
    glutDisplayFunc(displayXZ);

    consoleWindow = glutCreateSubWindow(mainWindow,
            10, 10,
            ConsoleWindow.x,
            ConsoleWindow.y);
    initConsoleWindow();
    glutDisplayFunc(displayConsole);

    glutKeyboardFunc(keyFunc);

    atexit(exitFunc);

    glutMainLoop();
    return 0;
}
