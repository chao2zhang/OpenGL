#include "dataManager.h"
#include "gl.h"
#include <fstream>
#include <iomanip>

using namespace std;

const char FILE_NAME[] = "data.txt";

void DataManager::load(vector<Polygon>& list) {
    ifstream in(FILE_NAME);
    if (!in)
        return;
    list.clear();
    int n;
    in >> n;
    for (int i = 0; i < n; i++) {
        int v;
        in >> v;
        Point p;
        Polygon pg;
        for (int j = 0; j < v; j++) {
            in >> p;
            pg.push(p);
        }
        list.push_back(pg);
    }
    in.close();
}
void DataManager::dump(const vector<Polygon>& list) {
    ofstream out(FILE_NAME);
    if (!out)
        return;
    out << list.size() << endl << endl;
    out << setprecision(1) << setiosflags(ios::fixed);
    for (int i = 0; i < list.size(); i++) {
        const vector<Point>& v = list[i].list();
        out << v.size() << endl;
        for (int j = 0; j < v.size(); j++) {
            out << v[j] << endl;
        }
        out << endl;
    }
    out.close();
}
