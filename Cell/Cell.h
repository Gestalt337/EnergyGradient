//
// Created by haoyuan on 03/11/24.
//

#ifndef CELL_H
#define CELL_H
#include "../Vertex/Vertex.h"
#include "../Polygon/Polygon.h"
#include "../Run/Box.h"
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>

using namespace std;

struct cpoly{
    int o;
    Polygon* p;
};

class Cell {
public:
    vector<Vertex*> vertices_;
    const int id_;
    //vector<Polygon*> polygons_;
    vector<cpoly> polygons_;
    array<double, 3> center_;
    double energy_=0.;
    double volume_=0.;
    double area_=0.;
    Box* box_;

    Cell(const int id, const vector<Vertex*>& vertices): vertices_(vertices), id_(id) {}

    int addBox(Box*);
    int getCenter();
    int getVolume();
    int getArea();
    int addPolygon(Polygon*);
    cpoly getCpoly(int);
};



#endif //CELL_H
