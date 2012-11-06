#include "dataManager.h"
#include "gl.h"
#include <fstream>
#include <iomanip>

using namespace std;

void DataManager::load(const char filename[], vector<Polygon>& list) {
    list.clear();
    ifstream in(filename);
    if (!in)
        return;
    int n;
    in >> n;
    for (int i = 0; i < n; i++) {
        Polygon pg(i);
        //Read vertices
        int v;
        in >> v;
        for (int j = 0; j < v; j++) {
            Point3f p;
            in >> p;
            pg.addVertex3f(p);
        }
        //Read edges
        int e;
        in >> e;
        for (int j = 0; j < e; j++) {
            int u, w;
            in >> u >> w;
            pg.addEdge2i(make_pair(u, w));
        }
        list.push_back(pg);
    }
    in.close();
}
void DataManager::dump(const char filename[], const vector<Polygon>& list) {
    ofstream out(filename);
    if (!out)
        return;
    out << list.size() << endl << endl;
    out << setprecision(3) << setiosflags(ios::fixed);
    for (int i = 0; i < list.size(); i++) {
        const vector<Point3f>& v = list[i].vertices();
        out << v.size() << endl;
        for (int j = 0; j < v.size(); j++) {
            out << v[j] << endl;
        }
        const vector<Edge>& e = list[i].edges();
        out << e.size() << endl;
        for (int j = 0; j < e.size(); j++) {
            out << e[j].first << ' ' << e[j].second << endl;
        }
        out << endl;
    }
    out.close();
}
