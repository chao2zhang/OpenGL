#include "console.h"
#include "hint.h"
#include <string>
#include <vector>
#include <sstream>

using namespace std;

State state = WAIT_STATE;
string hint = TEXT_OPTIONS_DETAILS;
string hintInput = TEXT_WAIT_OPTION;
string currInput;

void (*translateCallback)(const Point3f& v);
void (*rotateCallback)(const Point3f& v1, const Point3f& v2, float a);
void (*scaleCallback)(float a);
void (*nextCallback)();
void (*prevCallback)();

static Point3f translateVector;
static Point3f rotatePoint1;
static Point3f rotatePoint2;
static float rotateAngle;
static float scaleRatio;

void inputCharacter(unsigned char c) {
    switch(c) {
        case '\r':
            updateState();
            break;
        case '\b':
            if (currInput.size()) currInput.erase(currInput.size() - 1, 1);
            break;
        default:
            currInput += c;
            break;
    }
}

static void setState(State st) {
    state = st;
    switch(st) {
        case WAIT_STATE:
            hintInput = TEXT_WAIT_OPTION;
        break;
        case TRANSLATE_STATE:
            hintInput = TEXT_TRANS_VECTOR;
        break;
        case ROTATE_STATE_1:
            hintInput = TEXT_ROTATE_POINT1;
        break;
        case ROTATE_STATE_2:
            hintInput = TEXT_ROTATE_POINT2;
        break;
        case ROTATE_STATE_3:
            hintInput = TEXT_ROTATE_POINT3;
        break;
        case SCALE_STATE:
            hintInput = TEXT_SCALE_RATIO;
        break;
    }
}

void updateState() {
    if (currInput.size()) {
        stringstream out(currInput);
        if (state == WAIT_STATE) {
            switch(currInput[0]) {
                case 't': case 'T':
                    setState(TRANSLATE_STATE);
                    break;
                case 'r': case 'R':
                    setState(ROTATE_STATE_1);
                    break;
                case 's': case 'S':
                    setState(SCALE_STATE);
                    break;
                case 'n': case 'N':
                    nextCallback();
                    break;
                case 'p': case 'P':
                    prevCallback();
                    break;
            }
        } else if (state == TRANSLATE_STATE) {
            out >> translateVector;
            if (translateCallback)
                translateCallback(translateVector);
            setState(WAIT_STATE);
        } else if (state == ROTATE_STATE_1) {
            out >> rotatePoint1;
            setState(ROTATE_STATE_2);
        } else if (state == ROTATE_STATE_2) {
            out >> rotatePoint2;
            setState(ROTATE_STATE_3);
        } else if (state == ROTATE_STATE_3) {
            out >> rotateAngle;
            if (rotateCallback)
                rotateCallback(rotatePoint1, rotatePoint2, rotateAngle);
            setState(WAIT_STATE);
        } else if (state == SCALE_STATE) {
            out >> scaleRatio;
            if (scaleCallback)
                scaleCallback(scaleRatio);
            setState(WAIT_STATE);
        }
    }
    currInput.clear();
}
