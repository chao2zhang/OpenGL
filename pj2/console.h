#ifndef MENU_H
#define MENU_H

#include "gl.h"
#include <string>
#include <vector>

enum State{
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
extern void (*scaleCallback)(float a);
extern void (*nextCallback)();
extern void (*prevCallback)();

#endif
