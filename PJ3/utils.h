#ifndef UTILS_H
#define UTILS

#include <string>
#include "point.h"

std::string format(const std::string &fmt, ...);
void print(void* font, const std::string& s, Point2i& p, const Point2i& window, int h = 13);

#endif
