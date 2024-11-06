//
// Created by haoyuan on 03/11/24.
//

#ifndef POLYGON_H
#define POLYGON_H
#include "../Vertex/Vertex.h"
#include "../Edge/Edge.h"
#include <utility>
#include <vector>
#include <cmath>

#include "../Run/Box.h"

using namespace std;

struct cedge{
    int o;
    Edge* e;
};

class Polygon {
public:
    vector<Vertex*> vertices_;
    vector<cedge> edges_;
    const int id_;
    array<double, 3> center_{};
    vector<int> cellIds_={};
    bool crossBoundary_=false;
    Box* box_{};

    Polygon(const int id, vector<Vertex*> vertices): vertices_(move(vertices)), id_(id) {}

    int addBox(Box*);
    int getCenter();
    int checkBoundary();
    Vertex* getVertexPtr(int);
};


#endif //POLYGON_H
