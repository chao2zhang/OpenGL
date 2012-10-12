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

using namespace std;

enum MenuValue{
    MenuNext,
    MenuPrev,
    MenuTrans,
    MenuRot,
    MenuScale,
    MenuClip,
    MenuViewport,
    MenuViewportSize,
    MenuViewportPosition,
    MenuFast,
    MenuSave,
} currMenu;

int menu, subMenu;
DataManager manager;
vector<Polygon> polygons;
int currPolygon = 0;
int currMode = 0;
Color currColor = Color(0xff, 0xff, 0xff);
Color commColor = Color(0x11, 0x22, 0xcc);
GLubyte* pixels = new GLubyte[GWindow.size() * 3];

void nextFunc() {
    currPolygon = currPolygon == polygons.size() - 1 ? 0 : currPolygon + 1;
    glutPostOverlayRedisplay();
}

void prevFunc() {
    currPolygon = currPolygon == 0 ? polygons.size() - 1 : currPolygon - 1;
    glutPostOverlayRedisplay();
}

void transFunc(unsigned char key) {
    Point p;
    switch (key) {
        case 'h':
            p = Point(currMode ? -10 : -1, 0);
        break;
        case 'j':
            p = Point(0, currMode ? -10 : -1);
        break;
        case 'k':
            p = Point(0, currMode ? 10 : 1);
        break;
        case 'l':
            p = Point(currMode ? 10 : 1, 0);
        break;
    }
    polygons[currPolygon].translate(p.x, p.y);
}

void rotFunc(unsigned char key) {
    switch (key) {
        case 'h':case 'j':
            polygons[currPolygon].rotate(currMode ? 0.15 : 0.03);
        break;
        case 'k':case 'l':
            polygons[currPolygon].rotate(currMode ? -0.15 : -0.03);
        break;
    }
}

void scaleFunc(unsigned char key) {
    switch (key) {
        case 'h':case 'j':
            polygons[currPolygon].scale(currMode ? 1.05 : 1.025);
        break;
        case 'k':case 'l':
            polygons[currPolygon].scale(currMode ? 0.9524 : 0.9756);
        break;
    }
}

void (*currTransform)(unsigned char key) = transFunc;

void keyFunc(unsigned char key, int x, int y) {
    switch (key) {
        case 'h': case 'j': case'k': case'l':
            currTransform(key);
            glutPostRedisplay();
            break;
        case 'n':
            nextFunc();
            break;
        case 'p':
            prevFunc();
            break;
        case 't':
            currTransform = transFunc;
            break;
        case 'r':
            currTransform = rotFunc;
            break;
        case 's':
            currTransform = scaleFunc;
            break;
        case 'f':
            currMode = !currMode;
            break;
        case 'w':
            manager.dump(polygons);
            break;
    }
}

void subMenuFunc(int data) {
    switch(data) {
        case MenuViewportPosition:
        case MenuViewportSize:
            currMenu = (MenuValue)data;
            break;
    }
}

void menuFunc(int data) {
    switch(data) {
        case MenuNext:
            nextFunc();
            break;
        case MenuPrev:
            prevFunc();
            break;
        case MenuTrans:
            currTransform = transFunc;
            break;
        case MenuRot:
            currTransform = rotFunc;
            break;
        case MenuScale:
            currTransform = scaleFunc;
            break;
        case MenuFast:
            currMode = !currMode;
            break;
        case MenuClip:
            break;
        case MenuSave:
            manager.dump(polygons);
            break;
    }
}

void display() {
    memset(pixels, 0, GWindow.size() * 3);
    for (int i = 0; i < polygons.size(); i++) {
        if (i == currPolygon)
            makeColor(currColor);
        else
            makeColor(commColor);
        polygons[i].draw(pixels);
    }
    glDrawPixels(GWindow.width, GWindow.height, GL_RGB, GL_UNSIGNED_BYTE, pixels);
    glutSwapBuffers();
}

void idleFunc() {
    glutPostRedisplay();
}

void exitFunc() {
    delete [] pixels;
    manager.dump(polygons);
}

int main(int argc, char** argv) {
    manager.load(polygons);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(GWindow.width, GWindow.height);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("OpenGL glDrawPixels demo");
    glutDisplayFunc(display);
    glutKeyboardFunc(keyFunc);
    glutIdleFunc(idleFunc);

    subMenu = glutCreateMenu(subMenuFunc);
    glutAddMenuEntry("Viewport Size", MenuViewportSize);
    glutAddMenuEntry("Viewport Position", MenuViewportPosition);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    menu = glutCreateMenu(menuFunc);
    glutAddMenuEntry("Next Polygon(N)", MenuNext);
    glutAddMenuEntry("Prev Polygon(P)", MenuPrev);
    glutAddMenuEntry("Translate(T)", MenuTrans);
    glutAddMenuEntry("Rotate(R)", MenuRot);
    glutAddMenuEntry("Scale(S)", MenuScale);
    glutAddMenuEntry("Clip(X)", MenuClip);
    glutAddSubMenu("Viewport", subMenu);
    glutAddMenuEntry("Fast Mode On/Off(F)", MenuFast);
    glutAddMenuEntry("Save(W)", MenuSave);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    atexit(exitFunc);

    glutMainLoop();
    return 0;
}
