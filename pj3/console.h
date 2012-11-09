#ifndef MENU_H
#define MENU_H

#include "gl.h"
#include <string>
#include <vector>
#include <sstream>

enum State{
    FILE_STATE,
    WAIT_STATE,
    TRANSLATE_STATE,
    ROTATE_STATE_1,
    ROTATE_STATE_2,
    ROTATE_STATE_3,
    SCALE_STATE,
};

extern State state;

extern std::string hint;
extern std::string hintInput;
extern std::string currInput;

extern void updateState();
extern void inputCharacter(unsigned char c);

extern void (*translateCallback)(const Point3f& v);
extern void (*rotateCallback)(const Point3f& v1, const Point3f& v2, float a);
extern void (*rotate1Callback)(const Point3f& v1);
extern void (*rotate2Callback)(const Point3f& v1, const Point3f& v2);
extern void (*lightCallback)(const Point3f& v);
extern void (*scaleCallback)(float a);
extern void (*nextCallback)();
extern void (*prevCallback)();
extern void (*loadCallback)(const std::string& filename);
extern void (*saveCallback)();
extern void (*normalizeCallback)();

#endif
