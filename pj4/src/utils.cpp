#include "utils.h"
#include <typeinfo>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <GL/glut.h>

using namespace std;

int state = NORMAL_STATE;
vector<Curve*> curves;
int curId = 0;
int curPoint = 0;
void (*inputCallback)(const Point2f& p);
Point2f inputPoint;
bool knotInput = false;
int knotPos = false;
string knotString;

void refreshFunc() {
    glutPostWindowRedisplay(window);
    glutPostWindowRedisplay(knotWindow);
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

void mouseKnotFunc(int button, int mstate, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && mstate == GLUT_DOWN) {
        if (curves[curId]->name() == BSPLINE_NAME) {
            Bspline* b = dynamic_cast<Bspline*>(curves[curId]);
            knotPos = y / 15 - 1;
            if (0 <= knotPos && knotPos < b->knotSize()) {
                knotInput = true;
                knotString.clear();
            }
        }
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
    refreshFunc();
}

void menuFunc(int menu) {
    switch (menu) {
    case 1:
        load(curves);
        break;
    case 2:
        save(curves);
        break;
    }
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
        curves.push_back(new Bspline());
    break;
    case 5:
        Curve* curve = curves[curId];
        curves.erase(curves.begin() + curId);
        delete curve;
    break;
    }
    refreshFunc();
}

void menuBsplineFunc(int menu) {
    switch (menu) {
    case 1:
        if (curves[curId]->name() == BSPLINE_NAME)
            dynamic_cast<Bspline*>(curves[curId])->addK(1);
    break;
    case 2:
        if (curves[curId]->name() == BSPLINE_NAME)
            dynamic_cast<Bspline*>(curves[curId])->addK(-1);
    break;
    }
    refreshFunc();
}

void keyFunc(unsigned char ch, int x, int y) {
    switch (ch) {
        case 'N': case 'n': menuPointFunc(1); break;
        case 'P': case 'p': menuPointFunc(2); break;
        case 'A': case 'a': menuPointFunc(3); break;
        case 'R': case 'r': menuPointFunc(4); break;
        case 'I': case 'i': menuPointFunc(5); break;
        case 'M': case 'm': menuPointFunc(6); break;
        case 'L': case 'l': menuFunc(1); break;
        case 'S': case 's': menuFunc(2); break;
        case 'X': case 'x': menuIdFunc(1); break;
        case 'V': case 'v': menuIdFunc(2); break;
        case '=': case '+': menuBsplineFunc(1); break;
        case '_': case '-': menuBsplineFunc(2); break;
    }
    refreshFunc();
}

void keyKnotFunc(unsigned char ch, int x, int y) {
    switch (ch) {
        case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8':
        case '9': case '0': case '.':
            if (knotInput) knotString += ch;
            break;
        case '\r':
            if (knotInput) knotInput = false;
            if (curves[curId]->name() == BSPLINE_NAME)
                dynamic_cast<Bspline*>(curves[curId])->setKnot(knotPos, getFloat(knotString));
            break;
        case '\b':
            if (knotInput && knotString.size()) knotString.erase(knotString.size() - 1);
            break;
        case '\033':
            if (knotInput) knotInput = false;
    }
    refreshFunc();
}

void print(void* format, const string& str) {
    for (int i = 0; i < str.size(); i++)
        glutBitmapCharacter(format, str[i]);
}

void print(void* format, const char str[]) {
    while (*str) glutBitmapCharacter(format, *str++);
}

void print(void* format, int num) {
    char buf[10];
    sprintf(buf, "%d", num);
    print(format, string(buf));
}

void print(void* format, float num) {
    char buf[10];
    sprintf(buf, "%.3f", num);
    print(format, string(buf));
}

float getFloat(const string& str) {
    float ret;
    sscanf(str.c_str(), "%f", &ret);
    return ret;
}

void load(vector<Curve*>& curves) {
    ifstream in(FILENAME);
    while (curves.size()) {delete curves.back(); curves.pop_back();}
    int t; in >> t;
    for (int i = 0; i < t; i++) {
        int n, k;
        in >> n >> k;
        if (2 <= k && k <= n) {
            Bspline* c = new Bspline();
            for (int i = 0; i < n; i++) {float x, y; in >> x >> y; c->add(Point2f(x, y));}
            c->setK(k);
            for (int i = 0; i < n + k; i++) {float u; in >> u; c->setKnot(i, u);}
            curves.push_back(c);
        } else {
            Bezier* c = new Bezier();
            for (int i = 0; i < n; i++) {float x, y; in >> x >> y; c->add(Point2f(x, y));}
            curves.push_back(c);
        }
    }
}

void save(const vector<Curve*>& curves) {
    ofstream out(FILENAME);
    out << curves.size() << endl << endl;
    for (int i = 0; i < curves.size(); i++) {
        if (curves[i]->name() == BSPLINE_NAME) {
            Bspline* c = dynamic_cast<Bspline*>(curves[i]);
            out << c->size() << ' ' << c->getK() << endl;
            for (int i = 0; i < c->size(); i++) {out << c->get(i).x << ' ' << c->get(i).y << endl;}
            for (int i = 0; i < c->size() + c->getK(); i++) {out << c->getKnot(i) << ' ';}
            out << endl << endl;
        } else {
            Curve* c = curves[i];
            out << c->size() << ' ' << 0 << endl;
            for (int i = 0; i < c->size(); i++) {out << c->get(i).x << ' ' << c->get(i).y << endl;}
        }
    }
}
