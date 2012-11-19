#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <GL/glut.h>
#include <GL/glu.h>
#include "point.h"
#include "curve.h"

using namespace std;

const int WINDOW_SIZE = 600;
const int INPUT_STATE = 1;
const int NORMAL_STATE = 0;
const Point2i Window(WINDOW_SIZE, WINDOW_SIZE);

int window;
vector<Curve*> curves;
int curId = 0;
int curPoint = 0;
int menuId, menuPoint, menu;
int state = NORMAL_STATE;
void (*inputCallback)(const Point2f& p);
Point2f inputPoint;

string getString(int n) {
    stringstream ss;
    ss << n;
    return ss.str();
}

void initWindow() {
    glClearColor(0, 0, 0, 0);
    gluOrtho2D(0, 1, 0, 1);
}

void displayFunc() {
    glClear(GL_COLOR_BUFFER_BIT);
    glPointSize(1.0);
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_POINTS);
        for (int i = 0; i < curves.size(); i++) {
            for (float t = 0; t <= 1; t += 0.002)
                curves[i]->show(t);
        }
    glEnd();
    glPointSize(3.0);
    glBegin(GL_POINTS);
        if (curves.size())
            for (int j = 0; j < curves[curId]->size(); j++) {
                const Point2f& p = curves[curId]->get(j);
                glVertex2f(p.x, p.y);
            }
    glEnd();
    if (curves.size())
        for (int j = 0; j < curves[curId]->size(); j++) {
            const Point2f& p = curves[curId]->get(j);
            glRasterPos2f(p.x - 0.005, p.y + 0.01);
            const string& str = getString(j);
            for (int i = 0; i < str.size(); i++)
                glutBitmapCharacter(GLUT_BITMAP_9_BY_15, str[i]);
        }
    glPointSize(5.0);
    glBegin(GL_POINTS);
        if (curves.size() && curves[curId]->size()) {
            const Point2f& p = curves[curId]->get(curPoint);
            glVertex2f(p.x, p.y);
        }
    glEnd();
    if (state == INPUT_STATE) {
        glColor3f(1.0, 0.5, 0.5);
        glPointSize(7.0);
        glBegin(GL_POINTS); glVertex2f(inputPoint.x, inputPoint.y); glEnd();
    }
    glutSwapBuffers();
}

void refreshFunc() {
    glutPostWindowRedisplay(window);
}

void keyFunc(unsigned char ch, int x, int y) {
    refreshFunc();
}

void init(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(Window.x, Window.y);
    glutInitWindowPosition(100, 100);
}

void menuFunc(int menu) {
}

void menuIdFunc(int menu) {
    switch (menu) {
    case 1:
        curId += 1;
        curId %= curves.size();
        curPoint = 0;
    break;
    case 2:
        curId += curves.size() - 1;
        curId %= curves.size();
        curPoint = 0;
    break;
    case 3:
        curves.push_back(new Bezier());
    break;
    case 4:
    break;
    case 5:
        //curves.erase(curId);
    break;
    }
}

void setInputState() {
    state = INPUT_STATE;
}

void insertCallback(const Point2f& p) {
    curves[curId]->insert(curPoint, p);
}

void modifyCallback(const Point2f& p) {
    curves[curId]->set(curPoint, p);
}

void addCallback(const Point2f& p) {
    curves[curId]->add(p);
}

void mouseFunc(int button, int mstate, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && mstate == GLUT_DOWN && state == INPUT_STATE) {
        inputCallback(inputPoint);
        state = NORMAL_STATE;
    }
    refreshFunc();
}

void motionFunc(int x, int y) {
    inputPoint.x = (float)x / WINDOW_SIZE;
    inputPoint.y = 1 - (float)y / WINDOW_SIZE;
    if (state == INPUT_STATE && inputCallback == modifyCallback)
        inputCallback(inputPoint);
    refreshFunc();
}

void menuPointFunc(int menu) {
    switch (menu) {
    case 1:
        if (curves[curId]->size()) {
            curPoint += 1;
            curPoint %= curves[curId]->size();
        }
    break;
    case 2:
        if (curves[curId]->size()) {
            curPoint += curves[curId]->size() - 1;
            curPoint %= curves.size();
        }
    break;
    case 3:
        inputCallback = addCallback;
        setInputState();
    break;
    case 4:
        if (curves[curId]->size()) {
            curves[curId]->remove(curPoint);
        }
    break;
    case 5:
        inputCallback = insertCallback;
        setInputState();
    break;
    case 6:
        inputCallback = modifyCallback;
        setInputState();
    break;
    }
}

int main(int argc, char** argv) {
    init(argc, argv);
    window = glutCreateWindow("OpenGL Project 4");
    initWindow();
    curves.push_back(new Bezier());
    curves.back()->add(Point2f(0.3, 0.3));
    curves.back()->add(Point2f(0.5, 0.5));
    curves.back()->add(Point2f(0.3, 0.5));
    glutDisplayFunc(displayFunc);
    glutMouseFunc(mouseFunc);
    glutPassiveMotionFunc(motionFunc);
    menuId = glutCreateMenu(menuIdFunc);
    glutAddMenuEntry("Next Curve", 1);
    glutAddMenuEntry("Prev Curve", 2);
    glutAddMenuEntry("Add Bezier", 3);
    glutAddMenuEntry("Add Bspline", 4);
    glutAddMenuEntry("Remove Current Curve", 5);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    menuPoint = glutCreateMenu(menuPointFunc);
    glutAddMenuEntry("Next Point", 1);
    glutAddMenuEntry("Prev Point", 2);
    glutAddMenuEntry("Add Point", 3);
    glutAddMenuEntry("Remove Current Point", 4);
    glutAddMenuEntry("Insert Point", 5);
    glutAddMenuEntry("Modify Current Point", 6);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    menu = glutCreateMenu(menuFunc);
    glutAddSubMenu("Curve", menuId);
    glutAddSubMenu("Point", menuPoint);
    glutAddMenuEntry("Save", 1);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    glutMainLoop();
    return 0;
}
