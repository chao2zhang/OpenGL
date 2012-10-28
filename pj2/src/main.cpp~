#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <ctime>
#include <iostream>
#include <vector>
#include "gl.h"
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

std::string file;
DataManager manager;
vector<Polygon> polygons;
int currPolygon = 0;
int currMode = 0;
const GLubyte currColor[] = {0xdd, 0x4b, 0x39};
const GLubyte commColor[] = {0x9f, 0xb6, 0xcd};
const GLubyte rotPoint1Color[] = {0xff, 0x99, 0x00};
const GLubyte rotPoint2Color[] = {0xff, 0x33, 0x00};
const GLubyte rotLineColor[] = {0xff, 0xcc, 0x00};
Point3f* rotPoint1 = NULL;
Point3f* rotPoint2 = NULL;

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
    p.x = Window.x / 4 + SubWindowOffset.x; p.y = Window.y / 2 + TextOffset.y;
    print(GLUT_BITMAP_8_BY_13, "YZ-Plane", p, Window);
    p.x = Window.x * 2 / 4 + SubWindowOffset.x; p.y = Window.y / 2 + TextOffset.y;
    print(GLUT_BITMAP_8_BY_13, "XZ-Plane", p, Window);
    p.x = Window.x * 3 / 4 + SubWindowOffset.x; p.y = Window.y / 2 + TextOffset.y;
    print(GLUT_BITMAP_8_BY_13, "Cabinet-XY", p, Window);
    p.x = SubWindowOffset.x; p.y = TextOffset.y;
    print(GLUT_BITMAP_8_BY_13, "Cavalier-XY", p, Window);
    p.x = Window.x / 4 + SubWindowOffset.x; p.y = TextOffset.y;
    print(GLUT_BITMAP_8_BY_13, "Perspective(For Debug)", p, Window);
    glutSwapBuffers();
}

void displayXY() {
    glClear(GL_COLOR_BUFFER_BIT);
    for (int i = 0; i < polygons.size(); i++) {
        glColor3ubv(i == currPolygon ? currColor : commColor);
        polygons[i].project2D(PROJ_XY, SubWindow);
    }
    if (rotPoint1) {
        glColor3ubv(rotPoint1Color);
        glBegin(GL_POINTS);
        rotPoint1->project2D(PROJ_XY, SubWindow);
        glEnd();
    }
    if (rotPoint2) {
        glColor3ubv(rotPoint2Color);
        glBegin(GL_POINTS);
        rotPoint2->project2D(PROJ_XY, SubWindow);
        glEnd();
        glColor3ubv(rotLineColor);
        glBegin(GL_LINES);
            rotPoint1->project2D(PROJ_XY, SubWindow);
            rotPoint2->project2D(PROJ_XY, SubWindow);
        glEnd();
    }
    glutSwapBuffers();
}

void displayYZ() {
    glClear(GL_COLOR_BUFFER_BIT);
    for (int i = 0; i < polygons.size(); i++) {
        glColor3ubv(i == currPolygon ? currColor : commColor);
        polygons[i].project2D(PROJ_YZ, SubWindow);
    }
    if (rotPoint1) {
        glColor3ubv(rotPoint1Color);
        glBegin(GL_POINTS);
        rotPoint1->project2D(PROJ_YZ, SubWindow);
        glEnd();
    }
    if (rotPoint2) {
        glColor3ubv(rotPoint2Color);
        glBegin(GL_POINTS);
        rotPoint2->project2D(PROJ_YZ, SubWindow);
        glEnd();
        glColor3ubv(rotLineColor);
        glBegin(GL_LINES);
            rotPoint1->project2D(PROJ_YZ, SubWindow);
            rotPoint2->project2D(PROJ_YZ, SubWindow);
        glEnd();
    }
    glutSwapBuffers();
}


void displayXZ() {
    glClear(GL_COLOR_BUFFER_BIT);
    for (int i = 0; i < polygons.size(); i++) {
        glColor3ubv(i == currPolygon ? currColor : commColor);
        polygons[i].project2D(PROJ_XZ, SubWindow);
    }
    if (rotPoint1) {
        glColor3ubv(rotPoint1Color);
        glBegin(GL_POINTS);
        rotPoint1->project2D(PROJ_XZ, SubWindow);
        glEnd();
    }
    if (rotPoint2) {
        glColor3ubv(rotPoint2Color);
        glBegin(GL_POINTS);
        rotPoint2->project2D(PROJ_XZ, SubWindow);
        glEnd();
        glColor3ubv(rotLineColor);
        glBegin(GL_LINES);
            rotPoint1->project2D(PROJ_XZ, SubWindow);
            rotPoint2->project2D(PROJ_XZ, SubWindow);
        glEnd();
    }
    glutSwapBuffers();
}

void displayCabinet() {
    glClear(GL_COLOR_BUFFER_BIT);
    for (int i = 0; i < polygons.size(); i++) {
        glColor3ubv(i == currPolygon ? currColor : commColor);
        polygons[i].projectCabinet(SubWindow);
    }
    if (rotPoint1) {
        glColor3ubv(rotPoint1Color);
        glBegin(GL_POINTS);
        rotPoint1->projectCabinet(SubWindow);
        glEnd();
    }
    if (rotPoint2) {
        glColor3ubv(rotPoint2Color);
        glBegin(GL_POINTS);
        rotPoint2->projectCabinet(SubWindow);
        glEnd();
        glColor3ubv(rotLineColor);
        glBegin(GL_LINES);
            rotPoint1->projectCabinet(SubWindow);
            rotPoint2->projectCabinet(SubWindow);;
        glEnd();
    }
    glutSwapBuffers();
}

void displayCavalier() {
    glClear(GL_COLOR_BUFFER_BIT);
    for (int i = 0; i < polygons.size(); i++) {
        glColor3ubv(i == currPolygon ? currColor : commColor);
        polygons[i].projectCavalier(SubWindow);
    }
    if (rotPoint1) {
        glColor3ubv(rotPoint1Color);
        glBegin(GL_POINTS);
        rotPoint1->projectCavalier(SubWindow);
        glEnd();
    }
    if (rotPoint2) {
        glColor3ubv(rotPoint2Color);
        glBegin(GL_POINTS);
        rotPoint2->projectCavalier(SubWindow);
        glEnd();
        glColor3ubv(rotLineColor);
        glBegin(GL_LINES);
            rotPoint1->projectCavalier(SubWindow);
            rotPoint2->projectCavalier(SubWindow);
        glEnd();
    }
    glutSwapBuffers();
}

void displayCamera() {
    /*
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);
    for (int i = 0; i < polygons.size(); i++) {
        glColor3ubv(i == currPolygon ? currColor : commColor);
        polygons[i].show();
    }
    if (rotPoint1) {
        glColor3ubv(rotPoint1Color);
        glBegin(GL_POINTS);
        rotPoint1->show();
        glEnd();
    }
    if (rotPoint2) {
        glColor3ubv(rotPoint2Color);
        glBegin(GL_POINTS);
        rotPoint2->show();
        glEnd();
        glColor3ubv(rotLineColor);
        glBegin(GL_LINES);
            rotPoint1->show();
            rotPoint2->show();
        glEnd();
    }
    glutSwapBuffers();
    */
}

void displayConsole() {
    glClear(GL_COLOR_BUFFER_BIT);
    Point2i p(0, ConsoleWindow.y - 13);
    if (!polygons.empty())
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
    clearRotPoints();
}

void next() {
    currPolygon = currPolygon == polygons.size() - 1 ? 0 : currPolygon + 1;
    clearRotPoints();
}

void translate(const Point3f& p) {
    polygons[currPolygon].translate(p);
    clearRotPoints();
}

void rotate1(const Point3f& p1) {
    rotPoint1 = new Point3f(p1);
}

void rotate2(const Point3f& p1, const Point3f& p2) {
    rotPoint2 = new Point3f(p2);
}

void rotate(const Point3f& p1, const Point3f& p2, float a) {
    polygons[currPolygon].rotate(p1, p2, a);
}

void scale(float a) {
    polygons[currPolygon].scale(a);
    clearRotPoints();
}

void load(const std::string& filename) {
    file = filename;
    manager.load(filename.c_str(), polygons);
}

void save() {
    manager.dump(file.c_str(), polygons);
}

void normalize() {
    float best = 0.0;
    for (int i = 0; i < polygons.size(); i++)
        for (int j = 0; j < polygons[i].vertices().size(); j++) {
            const Point3f& p = polygons[i].vertices()[j];
            if (abs(p.x - 0.5) > best) best = abs(p.x - 0.5);
            if (abs(p.y - 0.5) > best) best = abs(p.y - 0.5);
            if (abs(p.z - 0.5) > best) best = abs(p.z - 0.5);
        }
    Point3f center(0.5, 0.5, 0.5);
    for (int i = 0; i < polygons.size(); i++) {
        polygons[i].scale(center, 0.5 / best);
    }
}

void init(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(Window.x, Window.y);
    glutInitWindowPosition(100, 100);

    nextCallback = next;
    prevCallback = prev;
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
