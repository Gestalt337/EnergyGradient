//
// Created by haoyuan on 03/11/24.
//

#ifndef RUN_H
#define RUN_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <array>

#include "../Cell/Cell.h"
#include "../Edge/Edge.h"


class Run {
public:
    vector<Vertex> vertices_;
    vector<Cell> cells_;
    vector<Polygon> polygons_;
    vector<Edge> edges_;
    long int ITERS_=0;
    double energy_=0;
    double kv_, s0_, v0_, dt_;
    double mu_=1.0, ks_=1.0;
    Box* box_{};

    Run(const double kv, const double v0, const double s0, const double dt):kv_(kv), v0_(v0),s0_(s0), dt_(dt) {
        addVertices();
        addCells();
        addPolygonRun();
        addEdges();
        //addPolygonCell();
        // compute_force();
        // update_energy();
    }
    int addBox(Box*);
    int addCells();
    int addEdges();
    int addVertices();
    int addPolygonRun();
    int addPolygonCell();
    Vertex* getVertexPtr(int);
    Cell* getCellPtr(int);
    Polygon* getPolygonPtr(int);
    int computeCellCenters();
    int computePolygonCenters();
    int computeCellVA();
    int computeForce();
    int updateEnergy();
    int updateEdges();
    int getVertexVelocity();
    int updateCell();

};



#endif //RUN_H
