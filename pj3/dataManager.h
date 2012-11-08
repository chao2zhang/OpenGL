#ifndef _DATA_MANAGER_H
#define _DATA_MANAGER_H

#include <vector>
#include "object.h"

using namespace std;

class DataManager {
public:
    static void load(const char filename[],
                     vector<Point>& points,
                     vector<Triangle>& triangles,
                     int& n);
    static void dump(const char filename[],
                     const vector<Point>& points,
                     const vector<Triangle>& triangles,
                     int n);
};
#endif
