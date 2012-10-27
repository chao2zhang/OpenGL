#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <ctime>
#include <iostream>
#include <vector>
#include "gl.h"
#include "polygon.h"
#include "dataManager.h"
#include "console.h"

const Point2i Window(880, 440);
const Point2i SubWindow(200, 200);
const Point2i SubWindowOffset(10, 5);
const Point2i ConsoleWindow(420, 200);
const Point2i ConsoleWindowOffset(10, 5);
const Point2i TextOffset(0, 5);

const int INTERVAL = 50;

using namespace std;

int mainWindow;
int xyWindow, yzWindow, xzWindow;
int cabinetWindow;
int cavalierWindow;
int cameraWindow;
int consoleWindow;

DataManager manager;
vector<Polygon> polygons;
int currPolygon = 0;
int currMode = 0;
GLbyte currColor[] = {0xdd, 0x4b, 0x39};
GLbyte commColor[] = {0x9f, 0xb6, 0xcd};

void setMainWindow() {
    glClearColor(0.8, 0.8, 0.8, 0);
    gluOrtho2D(0, Window.x, 0, Window.y);
}

void setSubwindow() {
    glClearColor(1, 1, 1, 0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, SubWindow.x, 0, SubWindow.y);
}

void setCabinetWindow() {
    glClearColor(1, 1, 1, 0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(-CABINET_INV * SubWindow.x, SubWindow.x, -CABINET_INV * SubWindow.y, SubWindow.y);
}

void setCavalierWindow() {
    glClearColor(1, 1, 1, 0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(-CAVALIER_INV * SubWindow.x, SubWindow.x, -CAVALIER_INV * SubWindow.y, SubWindow.y);
}

void setCameraWindow() {
    glClearColor(1, 1, 1, 0);
    glMatrixMode(GL_MODELVIEW);
    gluLookAt(1.5, -1, 2, 0.5, 0.5, 0.5, -1, 2, 5);
    glMatrixMode(GL_PROJECTION);
    glFrustum(-0.5, 0.5, -0.5, 0.5, 2, 4);
}

void setConsoleWindow() {
    glClearColor(0, 0, 0, 0);
    gluOrtho2D(0, ConsoleWindow.x, 0, ConsoleWindow.y);
}

void displayMain() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3b(0, 0, 0);
    Point2i p;
    p.x = SubWindowOffset.x; p.y = Window.y / 2 + TextOffset.y;
    print(GLUT_BITMAP_8_BY_13, "XY-Plane", p, Window);
    p.x = Window.x / 4 + SubWindowOffset.x; p.y = Window.y / 2 + TextOffset.y;
    print(GLUT_BITMAP_8_BY_13, "YZ-Plane", p, Window);
    p.x = Window.x * 2 / 4 + SubWindowOffset.x; p.y = Window.y / 2 + TextOffset.y;
    print(GLUT_BITMAP_8_BY_13, "XZ-Plane", p, Window);
    p.x = Window.x * 3 / 4 + SubWindowOffset.x; p.y = Window.y / 2 + TextOffset.y;
    print(GLUT_BITMAP_8_BY_13, "Cabinet", p, Window);
    p.x = SubWindowOffset.x; p.y = TextOffset.y;
    print(GLUT_BITMAP_8_BY_13, "Cavalier", p, Window);
    p.x = Window.x / 4 + SubWindowOffset.x; p.y = TextOffset.y;
    print(GLUT_BITMAP_8_BY_13, "Perspective(Using OpenGL)", p, Window);
    glutSwapBuffers();
}


void displayXY() {
    glClear(GL_COLOR_BUFFER_BIT);
    for (int i = 0; i < polygons.size(); i++) {
        glColor3bv(i == currPolygon ? currColor : commColor);
        polygons[i].project2D(PROJ_XY, SubWindow);
    }
    glutSwapBuffers();
}

void displayYZ() {
    glClear(GL_COLOR_BUFFER_BIT);
    for (int i = 0; i < polygons.size(); i++) {
        glColor3bv(i == currPolygon ? currColor : commColor);
        polygons[i].project2D(PROJ_YZ, SubWindow);
    }
    glutSwapBuffers();
}


void displayXZ() {
    glClear(GL_COLOR_BUFFER_BIT);
    for (int i = 0; i < polygons.size(); i++) {
        glColor3bv(i == currPolygon ? currColor : commColor);
        polygons[i].project2D(PROJ_XZ, SubWindow);
    }
    glutSwapBuffers();
}

void displayCabinet() {
    glClear(GL_COLOR_BUFFER_BIT);
    for (int i = 0; i < polygons.size(); i++) {
        glColor3bv(i == currPolygon ? currColor : commColor);
        polygons[i].projectCabinet(SubWindow);
    }
    glutSwapBuffers();
}

void displayCavalier() {
    glClear(GL_COLOR_BUFFER_BIT);
    for (int i = 0; i < polygons.size(); i++) {
        glColor3bv(i == currPolygon ? currColor : commColor);
        polygons[i].projectCavalier(SubWindow);
    }
    glutSwapBuffers();
}

void displayCamera() {
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);
    for (int i = 0; i < polygons.size(); i++) {
        glColor3bv(i == currPolygon ? currColor : commColor);
        polygons[i].show();
    }
    glutSwapBuffers();
}

void displayConsole() {
    glClear(GL_COLOR_BUFFER_BIT);
    Point2i p(0, ConsoleWindow.y - 13);
    print(GLUT_BITMAP_8_BY_13, polygons[currPolygon].toString(), p, ConsoleWindow);
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
    glutPostWindowRedisplay(cabinetWindow);
    glutPostWindowRedisplay(cavalierWindow);
    glutPostWindowRedisplay(cameraWindow);
    glutPostWindowRedisplay(consoleWindow);
}

void keyFunc(unsigned char ch, int x, int y) {
    inputCharacter(ch);
    refreshFunc();
}

void exitFunc() {
    manager.dump("data.out", polygons);
}

void prev() {
    currPolygon = currPolygon ? currPolygon - 1 : polygons.size() - 1;
}

void next() {
    currPolygon = currPolygon == polygons.size() - 1 ? 0 : currPolygon + 1;
}

void translate(const Point3f& p) {
    polygons[currPolygon].translate(p);
}

void scale(float a) {
    polygons[currPolygon].scale(a);
}

void init(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(Window.x, Window.y);
    glutInitWindowPosition(100, 100);

    nextCallback = next;
    prevCallback = prev;
    translateCallback = translate;
    scaleCallback = scale;
}

int main(int argc, char** argv) {
    manager.load("data.in", polygons);

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
            Window.x / 4 + SubWindowOffset.x,
            SubWindowOffset.y,
            SubWindow.x,
            SubWindow.y);
    setSubwindow();
    glutDisplayFunc(displayYZ);
    glutKeyboardFunc(keyFunc);

    xzWindow = glutCreateSubWindow(mainWindow,
            Window.x / 4 * 2 + SubWindowOffset.x,
            SubWindowOffset.y,
            SubWindow.x,
            SubWindow.y);
    setSubwindow();
    glutDisplayFunc(displayXZ);
    glutKeyboardFunc(keyFunc);

    cabinetWindow = glutCreateSubWindow(mainWindow,
            Window.x / 4 * 3 + SubWindowOffset.x,
            SubWindowOffset.y,
            SubWindow.x,
            SubWindow.y);
    setCabinetWindow();
    glutDisplayFunc(displayCabinet);
    glutKeyboardFunc(keyFunc);

    cavalierWindow = glutCreateSubWindow(mainWindow,
            SubWindowOffset.x,
            Window.y / 2 + SubWindowOffset.y,
            SubWindow.x,
            SubWindow.y);
    setCavalierWindow();
    glutDisplayFunc(displayCavalier);

    cameraWindow = glutCreateSubWindow(mainWindow,
            Window.x / 4 + SubWindowOffset.x,
            Window.y / 2 + SubWindowOffset.y,
            SubWindow.x,
            SubWindow.y);
    setCameraWindow();
    glutDisplayFunc(displayCamera);
    glutKeyboardFunc(keyFunc);

    consoleWindow = glutCreateSubWindow(mainWindow,
            Window.x / 2 + ConsoleWindowOffset.x,
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
