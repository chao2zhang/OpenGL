#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <ctime>
#include <iostream>
#include <vector>
#include "hint.h"
#include "gl.h"
#include "object.h"
#include "dataManager.h"
#include "console.h"

using namespace std;

#define WINDOW_SIZE 600

const Point2i Window(WINDOW_SIZE, WINDOW_SIZE);
const Point2i ConsoleWindow(580, 100);
const Point2i ObjectWindow(580, 480);
const Point2i TextOffset(0, 5);

int mainWindow;
int frontWindow, backWindow, leftWindow, rightWindow, upWindow, downWindow;
int consoleWindow, objectWindow;

float canvas[WINDOW_SIZE * WINDOW_SIZE * 3];
Point3f *rotPoint1, *rotPoint2;

string file;
DataManager manager;

vector<Point> points;
vector<Triangle> triangles;
int n, curr;

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

void initObjectWindow() {
    glClearColor(0, 0, 0, 0);
    gluOrtho2D(0, ObjectWindow.x, 0, ObjectWindow.y);
}

void displayMain() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3b(0, 0, 0);
    glutSwapBuffers();
}

void displayFront() {
    cout << ">>>>BEGIN Front" << endl;
    glClear(GL_COLOR_BUFFER_BIT);
    memset(canvas, 0.0f, sizeof(canvas));
    normal(points, triangles, n);
    projectOrtho(points, PLANE_XZ);
    Point3f viewPoint(0.5, 0.5-dist, 0.5);
    Point3f viewVector(0, -1, 0);
    sort(points, triangles, viewPoint, viewVector);
    light(points, lightSource, viewPoint);
    triangle(points, triangles, canvas, WINDOW_SIZE);
    glDrawPixels(WINDOW_SIZE, WINDOW_SIZE, GL_RGB, GL_FLOAT, canvas);
    glutSwapBuffers();
    cout << "<<<<END Front" << endl;
}

void displayBack() {
    cout << ">>>>BEGIN Back" << endl;
    glClear(GL_COLOR_BUFFER_BIT);
    memset(canvas, 0.0f, sizeof(canvas));
    normal(points, triangles, n);
    projectOrtho(points, PLANE_ZX);
    Point3f viewPoint(0.5, 0.5+dist, 0.5);
    Point3f viewVector(0, 1, 0);
    sort(points, triangles, viewPoint, viewVector);
    light(points, lightSource, viewPoint);
    triangle(points, triangles, canvas, WINDOW_SIZE);
    glDrawPixels(WINDOW_SIZE, WINDOW_SIZE, GL_RGB, GL_FLOAT, canvas);
    glutSwapBuffers();
    cout << "<<<<END Back" << endl;
}


void displayLeft() {
    cout << ">>>>BEGIN LEFT" << endl;
    glClear(GL_COLOR_BUFFER_BIT);
    memset(canvas, 0.0f, sizeof(canvas));
    normal(points, triangles, n);
    projectOrtho(points, PLANE_ZY);
    Point3f viewPoint(0.5-dist, 0.5, 0.5);
    Point3f viewVector(-1, 0, 0);
    sort(points, triangles, viewPoint, viewVector);
    light(points, lightSource, viewPoint);
    triangle(points, triangles, canvas, WINDOW_SIZE);
    glDrawPixels(WINDOW_SIZE, WINDOW_SIZE, GL_RGB, GL_FLOAT, canvas);
    glutSwapBuffers();
    cout << "<<<<END LEFT" << endl;
}

void displayRight() {
    cout << ">>>>BEGIN RIGHT" << endl;
    glClear(GL_COLOR_BUFFER_BIT);
    memset(canvas, 0.0f, sizeof(canvas));
    normal(points, triangles, n);
    projectOrtho(points, PLANE_YZ);
    Point3f viewPoint(0.5+dist, 0.5, 0.5);
    Point3f viewVector(1, 0, 0);
    sort(points, triangles, viewPoint, viewVector);
    light(points, lightSource, viewPoint);
    triangle(points, triangles, canvas, WINDOW_SIZE);
    glDrawPixels(WINDOW_SIZE, WINDOW_SIZE, GL_RGB, GL_FLOAT, canvas);
    glutSwapBuffers();
    cout << "<<<<END RIGHT" << endl;
}

void displayTop() {
    cout << ">>>>BEGIN TOP" << endl;
    glClear(GL_COLOR_BUFFER_BIT);
    memset(canvas, 0.0f, sizeof(canvas));
    normal(points, triangles, n);
    projectOrtho(points, PLANE_XY);
    Point3f viewPoint(0.5, 0.5, 0.5+dist);
    Point3f viewVector(0, 0, 1);
    sort(points, triangles, viewPoint, viewVector);
    light(points, lightSource, viewPoint);
    triangle(points, triangles, canvas, WINDOW_SIZE);
    glDrawPixels(WINDOW_SIZE, WINDOW_SIZE, GL_RGB, GL_FLOAT, canvas);
    glutSwapBuffers();
    cout << "<<<<END TOP" << endl;
}

void displayBottom() {
    cout << ">>>>BEGIN BOTTOM" << endl;
    glClear(GL_COLOR_BUFFER_BIT);
    memset(canvas, 0.0f, sizeof(canvas));
    normal(points, triangles, n);
    projectOrtho(points, PLANE_YX);
    Point3f viewPoint(0.5, 0.5, 0.5-dist);
    Point3f viewVector(0, 0, -1);
    sort(points, triangles, viewPoint, viewVector);
    light(points, lightSource, viewPoint);
    triangle(points, triangles, canvas, WINDOW_SIZE);
    glDrawPixels(WINDOW_SIZE, WINDOW_SIZE, GL_RGB, GL_FLOAT, canvas);
    glutSwapBuffers();
    cout << "<<<<END BOTTOM" << endl;
}

string textParameter() {
    return format("g=%d h=%d l=%s dist=%.3f ka=%.3f kd=%.3f ks=%.3f k=%.3f ia=%s il=%s n=%d",
        grayScale,
        halfTone,
        lightSource.toString().c_str(),
        dist,
        kA,
        kD,
        kS,
        kK,
        iA.toString().c_str(),
        iL.toString().c_str(),
        pN);
}

void displayConsole() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);
    Point2i p(0, ConsoleWindow.y - 15);
    print(GLUT_BITMAP_8_BY_13, hint, p, ConsoleWindow);
    print(GLUT_BITMAP_8_BY_13,
          format(TEXT_OPTIONS_PARAMETERS, textParameter().c_str()),
          p, ConsoleWindow);
    print(GLUT_BITMAP_8_BY_13, hintInput, p, ConsoleWindow);
    print(GLUT_BITMAP_8_BY_13, currInput, p, ConsoleWindow);
    print(GLUT_BITMAP_8_BY_13, "_", p, ConsoleWindow);
    glutSwapBuffers();
}

void displayObject() {
    cout << ">>>>BEGIN OBJECT" << endl;
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);
    Point2i p(0, ObjectWindow.y - 15);
    string str = toString(points, triangles, curr);
    print(GLUT_BITMAP_8_BY_13, str, p, ObjectWindow);
    //cout << str << endl;
    glutSwapBuffers();
    cout << "<<<<END OBJECT" << endl;
}

void refreshFunc() {
    glutPostWindowRedisplay(mainWindow);
    glutPostWindowRedisplay(frontWindow);
    glutPostWindowRedisplay(backWindow);
    glutPostWindowRedisplay(leftWindow);
    glutPostWindowRedisplay(rightWindow);
    glutPostWindowRedisplay(upWindow);
    glutPostWindowRedisplay(downWindow);
    glutPostWindowRedisplay(consoleWindow);
    glutPostWindowRedisplay(objectWindow);
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

void next() {
    curr++;
    curr %= n;
}

void prev() {
    curr += n - 1;
    curr %= n;
}

void translate(const Point3f& p) {
    translate(points, p, curr);
    clearRotPoints();
}

void rotate1(const Point3f& p1) {
    rotPoint1 = new Point3f(p1);
}

void rotate2(const Point3f& p1, const Point3f& p2) {
    rotPoint2 = new Point3f(p2);
}

void rotate(const Point3f& p1, const Point3f& p2, float a) {
    rotate(points, p1, p2, a, curr);
}

void scale(float a) {
    scale(points, a, curr);
    clearRotPoints();
}

void load(const std::string& filename) {
    file = filename;
    manager.load(file.c_str(), points, triangles, n);
    curr = 0;
}

void save() {
    manager.save(file.c_str(), points, triangles, n);
}

void normalize() {
    float best = 0.0;
    for (int i = 0; i < points.size(); i++) {
        const Point3f& p = points[i].point;
        if (abs(p.x - 0.5) > best) best = abs(p.x - 0.5);
        if (abs(p.y - 0.5) > best) best = abs(p.y - 0.5);
        if (abs(p.z - 0.5) > best) best = abs(p.z - 0.5);
    }
    Point3f center(0.5, 0.5, 0.5);
    scale(points, center, 0.5 / best);
}

void parameter(const string& param, const string& value) {
    stringstream ss(value);
    if (param == "ka") {
        ss >> kA;
    } else if (param == "kd") {
        ss >> kD;
    } else if (param == "ks") {
        ss >> kS;
    } else if (param == "k") {
        ss >> kK;
    }else if (param == "ia") {
        ss >> iA;
    } else if (param == "il") {
        ss >> iL;
    } else if (param == "n") {
        ss >> pN;
    } else if (param == "dist") {
        ss >> dist;
    } else if (param == "g") {
        grayScale = !grayScale;
    } else if (param == "h") {
        halfTone = !halfTone;
    } else if (param == "l") {
        ss >> lightSource;
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
    parameterCallback = parameter;
    getTextParameter = textParameter;
}

int main(int argc, char** argv) {

    init(argc, argv);

    mainWindow = glutCreateWindow("OpenGL Project 3");
    initMainWindow();
    glutDisplayFunc(displayMain);
    glutKeyboardFunc(keyFunc);

    frontWindow = glutCreateWindow("FRONT TO BACK(XZ)");
    initSubWindow();
    glutPositionWindow(200, 100);
    glutDisplayFunc(displayFront);

    backWindow = glutCreateWindow("BACK TO FRONT(ZX)");
    initSubWindow();
    glutPositionWindow(250, 100);
    glutDisplayFunc(displayBack);

    leftWindow = glutCreateWindow("LEFT TO RIGHT(ZY)");
    initSubWindow();
    glutPositionWindow(300, 100);
    glutDisplayFunc(displayLeft);

    rightWindow = glutCreateWindow("RIGHT TO LEFT(YZ)");
    initSubWindow();
    glutPositionWindow(350, 100);
    glutDisplayFunc(displayRight);

    upWindow = glutCreateWindow("TOP TO DOWN(XY)");
    initSubWindow();
    glutPositionWindow(400, 100);
    glutDisplayFunc(displayTop);

    downWindow = glutCreateWindow("BUTTOM TO TOP(YX)");
    initSubWindow();
    glutPositionWindow(450, 100);
    glutDisplayFunc(displayBottom);

    consoleWindow = glutCreateSubWindow(mainWindow,
            10, 10,
            ConsoleWindow.x,
            ConsoleWindow.y);
    initConsoleWindow();
    glutDisplayFunc(displayConsole);
    glutKeyboardFunc(keyFunc);

    objectWindow = glutCreateSubWindow(mainWindow,
            10, 120,
            ObjectWindow.x,
            ObjectWindow.y);
    initObjectWindow();
    glutDisplayFunc(displayObject);
    glutKeyboardFunc(keyFunc);

    atexit(exitFunc);

    glutMainLoop();
    return 0;
}
