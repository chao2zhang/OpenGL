#ifndef _DATA_MANAGER_H
#define _DATA_MANAGER_H

#include <vector>
#include <string>
#include "polygon.h"

class DataManager {
public:
    static void load(const char filename[], std::vector<Polygon>& list);
    static void dump(const char filename[], const std::vector<Polygon>& list);
};
#endif
