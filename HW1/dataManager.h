#ifndef _DATA_MANAGER_H
#define _DATA_MANAGER_H

#include <vector>
#include "polygon.h"

class DataManager {
public:
    static void load(std::vector<Polygon>& list);
    static void dump(const std::vector<Polygon>& list);
};
#endif
