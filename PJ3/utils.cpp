#include "utils.h"
#include <cstring>
#include <cmath>
#include <cstdio>
#include <cstdarg>

std::string format(const std::string &fmt, ...) {
    int size=100;
    std::string str;
    va_list ap;
    while (1) {
        str.resize(size);
        va_start(ap, fmt);
        int n = vsnprintf((char *)str.c_str(), size, fmt.c_str(), ap);
        va_end(ap);
        if (n > -1 && n < size) {
            str.resize(n);
            return str;
        }
        if (n > -1)
            size=n+1;
        else
            size*=2;
    }
}

void print(void* font, const std::string& str, Point2i& p, const Point2i& window, int h){
     glRasterPos2i(p.x, p.y);
     int dx = glutBitmapWidth(font, ' ');
     int c = 0, s = 0;
     for (int i = 0; i < str.size(); i++) {
        if (dx + p.x > window.x || str[i] == '\n') {
            p.x = 0; p.y -= h; c = 0;
            glRasterPos2i(p.x, p.y);
        }
        switch (str[i]) {
            case '\n':
                break;
            case '\t':
                s = (c / 4 + 1) * 4 - c;
                while (s--) {
                    p.x += dx; ++c;
                    glutBitmapCharacter(font, ' ');
                }
                break;
            default:
                p.x += dx; ++c;
                glutBitmapCharacter(font, str[i]);
                break;
        }
     }
}


