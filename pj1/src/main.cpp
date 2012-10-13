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
    MenuLeft,
    MenuDown,
    MenuUp,
    MenuRight,
    MenuTrans,
    MenuRot,
    MenuScale,
    MenuClip,
    MenuViewport,
    MenuViewportSize,
    MenuViewportPosition,
    MenuFast,
    MenuDb,
    MenuSave,
    MenuComment,
} currMenu;

int menu, subMenu;
DataManager manager;
vector<Polygon> polygons;
int currPolygon = 0;
int currMode = 0;
Color currColor = Color(0xdd, 0x4b, 0x39);
Color commColor = Color(0x9f, 0xb6, 0xcd);
GLubyte* pixels = new GLubyte[GWindow.size() * 3];

void trim(int &val, int min, int max) {
    if (val < min) val = min;
    if (val >= max) val = max;
}

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
            polygons[currPolygon].rotate(currMode ? 0.1 : 0.01);
        break;
        case 'k':case 'l':
            polygons[currPolygon].rotate(currMode ? -0.1 : -0.01);
        break;
    }
}

void scaleFunc(unsigned char key) {
    switch (key) {
        case 'h':case 'j':
            polygons[currPolygon].scale(currMode ? 0.9091 : 0.9901);
        break;
        case 'k':case 'l':
            polygons[currPolygon].scale(currMode ? 1.1 : 1.01);
        break;
    }
}

void viewportPositionFunc(unsigned char key) {
    switch (key) {
        case 'h':
            GViewport.left -= currMode ? 10 : 1;
            GViewport.right -= currMode ? 10 : 1;
        break;
        case 'j':
            GViewport.bottom -= currMode ? 10 : 1;
            GViewport.top -= currMode ? 10 : 1;
        break;
        case 'k':
            GViewport.bottom += currMode ? 10 : 1;
            GViewport.top += currMode ? 10 : 1;
        break;
        case 'l':
            GViewport.left += currMode ? 10 : 1;
            GViewport.right += currMode ? 10 : 1;
        break;
    }
    trim(GViewport.left, 0, GWindow.width);
    trim(GViewport.right, 0, GWindow.width);
    trim(GViewport.bottom, 0, GWindow.height);
    trim(GViewport.top, 0, GWindow.height);
}

void viewportSizeFunc(unsigned char key) {
    switch (key) {
        case 'h':
            GViewport.right -= currMode ? 10 : 1;
        break;
        case 'j':
            GViewport.top -= currMode ? 10 : 1;
        break;
        case 'k':
            GViewport.top += currMode ? 10 : 1;
        break;
        case 'l':
            GViewport.right += currMode ? 10 : 1;
        break;
    }
    trim(GViewport.left, 0, GWindow.width);
    trim(GViewport.right, 0, GWindow.width);
    trim(GViewport.bottom, 0, GWindow.height);
    trim(GViewport.top, 0, GWindow.height);
}

void (*currTransform)(unsigned char key) = transFunc;

void subMenuFunc(int data) {
    switch(data) {
        case MenuViewportPosition:
            currTransform = viewportPositionFunc;
            break;
        case MenuViewportSize:
            currTransform = viewportSizeFunc;
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
            for (int i = 0; i < polygons.size(); i++)
                polygons[i].clip(GViewport);
            break;
        case MenuDb:
            makeLine = (makeLine == makeLineDDA ? makeLineBres : makeLineDDA);
            break;
        case MenuSave:
            manager.dump("data.out", polygons);
            break;
        case MenuLeft:
            currTransform('h');
            glutPostRedisplay();
            break;
        case MenuDown:
            currTransform('j');
            glutPostRedisplay();
            break;
        case MenuUp:
            currTransform('k');
            glutPostRedisplay();
            break;
        case MenuRight:
            currTransform('l');
            glutPostRedisplay();
            break;

    }
}


void keyFunc(unsigned char key, int x, int y) {
    switch (key) {
        case 'h': case 'j': case'k': case'l':
            currTransform(key);
            glutPostRedisplay();
            break;
        case 'n':
            menuFunc(MenuNext);
            break;
        case 'p':
            menuFunc(MenuPrev);
            break;
        case 't':
            menuFunc(MenuTrans);
            break;
        case 'r':
            menuFunc(MenuRot);
            break;
        case 's':
            menuFunc(MenuScale);
            break;
        case 'f':
            menuFunc(MenuFast);
            break;
        case 'w':
            menuFunc(MenuSave);
            break;
        case 'z':
            subMenuFunc(MenuViewportPosition);
            break;
        case 'a':
            subMenuFunc(MenuViewportSize);
            break;
        case 'x':
            menuFunc(MenuClip);
            break;
        case 'd':
            menuFunc(MenuDb);
            break;
    }
}

void display() {
    clearPixel(pixels, GWindow.size() * 3);
    for (int i = 0; i < polygons.size(); i++) {
        if (i == currPolygon)
            makeColor(currColor);
        else
            makeColor(commColor);
        polygons[i].draw(pixels);
        polygons[i].fill(pixels);
    }
    projViewport(pixels);
    glDrawPixels(GWindow.width, GWindow.height, GL_RGB, GL_UNSIGNED_BYTE, pixels);
    glutSwapBuffers();
}

void idleFunc() {
    glutPostRedisplay();
}

void exitFunc() {
    delete [] pixels;
    manager.dump("data.out", polygons);
}

int main(int argc, char** argv) {
    manager.load("data.in", polygons);
    makeLine = makeLineDDA;
    makeViewport(20, 20, GWindow.width - 20, GWindow.height - 20);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(GWindow.width, GWindow.height);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("OpenGL glDrawPixels demo");
    glutDisplayFunc(display);
    glutKeyboardFunc(keyFunc);
    glutIdleFunc(idleFunc);

    subMenu = glutCreateMenu(subMenuFunc);
    glutAddMenuEntry("Viewport Size(Z)", MenuViewportSize);
    glutAddMenuEntry("Viewport Position(A)", MenuViewportPosition);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    menu = glutCreateMenu(menuFunc);
    glutAddMenuEntry("Next Polygon(N)", MenuNext);
    glutAddMenuEntry("Prev Polygon(P)", MenuPrev);
    glutAddMenuEntry("Left/C-Clockwise(H)", MenuLeft);
    glutAddMenuEntry("Down/C-Clockwise(J)", MenuDown);
    glutAddMenuEntry("Up/Clockwise(K)", MenuUp);
    glutAddMenuEntry("Right/Clockwise(L)", MenuRight);
    glutAddMenuEntry("Translate By +-1(T)", MenuTrans);
    glutAddMenuEntry("Rotate By +-0.01(R)", MenuRot);
    glutAddMenuEntry("Scale By +-0.1(S)", MenuScale);
    glutAddMenuEntry("x10 On/Off(F)", MenuFast);
    glutAddMenuEntry("Clip(X)", MenuClip);
    glutAddSubMenu("Viewport", subMenu);
    glutAddMenuEntry("DDA/Bresenham(D)", MenuDb);
    glutAddMenuEntry("Save(W)", MenuSave);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    atexit(exitFunc);

    glutMainLoop();
    return 0;
}
