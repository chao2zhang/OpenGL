#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <string>
#include "point.h"
#include "curve.h"

using namespace std;

extern const int WINDOW_SIZE;
extern int window;
extern int knotWindow;
extern vector<Curve*> curves;
extern int curId;
extern int curPoint;
extern void (*inputCallback)(const Point2f& p);
extern Point2f inputPoint;
extern int state;
const int INPUT_STATE = 1;
const int NORMAL_STATE = 0;
extern int knotPos;
extern bool knotInput;
extern string knotString;

void refreshFunc();
void setInputState();
void insertCallback(const Point2f& p);
void modifyCallback(const Point2f& p);
void addCallback(const Point2f& p);
void mouseFunc(int button, int mstate, int x, int y);
void mouseKnotFunc(int button, int mstate, int x, int y);
void motionFunc(int x, int y);
void menuPointFunc(int menu);
void menuFunc(int menu);
void menuIdFunc(int menu);
void menuBsplineFunc(int menu);
void keyFunc(unsigned char ch, int x, int y);
void keyKnotFunc(unsigned char ch, int x, int y);
void print(void* format, const string& str);
void print(void* format, char str[]);
void print(void* format, int num);
void print(void* format, float num);
float getFloat(const string& str);
#endif
