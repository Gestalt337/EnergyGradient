//
// Created by haoyuan on 03/11/24.
//

#include "Polygon.h"

#include <iostream>

int Polygon::getCenter() {
    double xs = 0., ys = 0., zs = 0.;
    int const ns = static_cast<int>(vertices_.size());
    for (int i=1; i<ns; ++i) {// This cannot use edge, because of the diferent algorithm.
        xs += box_->resetDistance(vertices_[i]->pos_[0] - vertices_[0]->pos_[0],0);
        ys += box_->resetDistance(vertices_[i]->pos_[1] - vertices_[0]->pos_[1],1);
        zs += box_->resetDistance(vertices_[i]->pos_[2] - vertices_[0]->pos_[2],2);
    }
    center_ = {xs/ns+vertices_[0]->pos_[0], ys/ns+vertices_[0]->pos_[1], zs/ns+vertices_[0]->pos_[2]};
    return 0;
}

Vertex* Polygon::getVertexPtr(int idn) {
    for (Vertex* vtx : vertices_) {
        if (vtx->id_ == idn) {
            return vtx;
        }
    }
    return nullptr;
}

int Polygon::addBox(Box *box) {
    box_ = box;
    return 0;
}

int Polygon::checkBoundary() {
    int np = static_cast<int>(vertices_.size());
    for (int i=0; i<np;++i) {
        double dx = fabs(vertices_[i]->pos_[0]-vertices_[(i+1)%np]->pos_[0]);
        double dy = fabs(vertices_[i]->pos_[1]-vertices_[(i+1)%np]->pos_[1]);
        double dz = fabs(vertices_[i]->pos_[2]-vertices_[(i+1)%np]->pos_[2]);
        if (dx>0.5*box_->boxSize_[0]||dy>0.5*box_->boxSize_[1]||dz>0.5*box_->boxSize_[2]) {
            crossBoundary_ = true;
        }
    }
    return 0;
}
