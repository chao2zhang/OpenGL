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

const Point2i Window(960, 660);
const Point2i SubWindow(300, 300);
const Point2i SubWindowOffset(10, 10);
const Point2i ConsoleWindow(940, 300);
const Point2i ConsoleWindowOffset(10, 10);
const Point2i TextOffset(0, 5);

int mainWindow;
int xyWindow, yzWindow, xzWindow;
int consoleWindow;

Point3f *rotPoint1, *rotPoint2;

std::string file;
DataManager manager;

void clearRotPoints() {
    if (rotPoint1) delete rotPoint1;
    if (rotPoint2) delete rotPoint2;
    rotPoint1 = rotPoint2 = NULL;
}

void setMainWindow() {
    glClearColor(0.8, 0.8, 0.8, 0);
    gluOrtho2D(0, Window.x, 0, Window.y);
}

void setSub() {
    glEnable(GL_PROGRAM_POINT_SIZE);
    glPointSize(3.0f);
}

void setSubwindow() {
    setSub();
    glClearColor(1, 1, 1, 0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, SubWindow.x, 0, SubWindow.y);
}

void setCabinetWindow() {
    setSub();
    glClearColor(1, 1, 1, 0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(-CABINET_INV * SubWindow.x, SubWindow.x, -CABINET_INV * SubWindow.y, SubWindow.y);
}

void setCavalierWindow() {
    setSub();
    glClearColor(1, 1, 1, 0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(-CAVALIER_INV * SubWindow.x, SubWindow.x, -CAVALIER_INV * SubWindow.y, SubWindow.y);
}

void setCameraWindow() {
    setSub();
    glClearColor(1, 1, 1, 0);
    glMatrixMode(GL_MODELVIEW);
    gluLookAt(1.5, -1, 2, 0.5, 0.5, 0.5, -1, 2, 5);
    glMatrixMode(GL_PROJECTION);
    glFrustum(-0.7, 0.7, -0.7, 0.7, 2, 4);
}

void setConsoleWindow() {
    setSub();
    glClearColor(0, 0, 0, 0);
    gluOrtho2D(0, ConsoleWindow.x, 0, ConsoleWindow.y);
}

void displayMain() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3b(0, 0, 0);
    Point2i p;
    p.x = SubWindowOffset.x; p.y = Window.y / 2 + TextOffset.y;
    print(GLUT_BITMAP_8_BY_13, "XY-Plane", p, Window);
    p.x = Window.x / 3 + SubWindowOffset.x; p.y = Window.y / 2 + TextOffset.y;
    print(GLUT_BITMAP_8_BY_13, "YZ-Plane", p, Window);
    p.x = Window.x * 2 / 3 + SubWindowOffset.x; p.y = Window.y / 2 + TextOffset.y;
    print(GLUT_BITMAP_8_BY_13, "XZ-Plane", p, Window);
    glutSwapBuffers();
}

void displayXY() {
    glClear(GL_COLOR_BUFFER_BIT);
    glutSwapBuffers();
}

void displayYZ() {
    glClear(GL_COLOR_BUFFER_BIT);
    glutSwapBuffers();
}


void displayXZ() {
    glClear(GL_COLOR_BUFFER_BIT);
    glutSwapBuffers();
}

void displayCabinet() {
    glClear(GL_COLOR_BUFFER_BIT);
    glutSwapBuffers();
}

void displayCavalier() {
    glClear(GL_COLOR_BUFFER_BIT);
    glutSwapBuffers();
}

void displayCamera() {
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);
    glutSwapBuffers();
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
}

void keyFunc(unsigned char ch, int x, int y) {
    inputCharacter(ch);
    refreshFunc();
}

void exitFunc() {
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
}

void save() {
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

    mainWindow = glutCreateWindow("OpenGL Project 2");
    setMainWindow();
    glutDisplayFunc(displayMain);
    glutKeyboardFunc(keyFunc);

    xyWindow = glutCreateSubWindow(mainWindow,
            SubWindowOffset.x,
            SubWindowOffset.y,
            SubWindow.x,
            SubWindow.y);
    setSubwindow();
    glutDisplayFunc(displayXY);
    glutKeyboardFunc(keyFunc);

    yzWindow = glutCreateSubWindow(mainWindow,
            Window.x / 3 + SubWindowOffset.x,
            SubWindowOffset.y,
            SubWindow.x,
            SubWindow.y);
    setSubwindow();
    glutDisplayFunc(displayYZ);
    glutKeyboardFunc(keyFunc);

    xzWindow = glutCreateSubWindow(mainWindow,
            Window.x / 3 * 2 + SubWindowOffset.x,
            SubWindowOffset.y,
            SubWindow.x,
            SubWindow.y);
    setSubwindow();
    glutDisplayFunc(displayXZ);
    glutKeyboardFunc(keyFunc);

    consoleWindow = glutCreateSubWindow(mainWindow,
            ConsoleWindowOffset.x,
            Window.y / 2 + ConsoleWindowOffset.y,
            ConsoleWindow.x,
            ConsoleWindow.y);
    setConsoleWindow();
    glutDisplayFunc(displayConsole);

    glutKeyboardFunc(keyFunc);

    atexit(exitFunc);

    glutMainLoop();
    return 0;
}
