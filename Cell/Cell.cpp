//
// Created by haoyuan on 03/11/24.
//

#include "Cell.h"

using namespace std;

int Cell::getCenter() {
    double xs = 0., ys = 0., zs = 0.;
    int const ns = static_cast<int>(vertices_.size());
    for (int i=1; i<ns; ++i) {
        xs += box_->resetDistance(vertices_[i]->pos_[0] - vertices_[0]->pos_[0],0);
        ys += box_->resetDistance(vertices_[i]->pos_[1] - vertices_[0]->pos_[1],1);
        zs += box_->resetDistance(vertices_[i]->pos_[2] - vertices_[0]->pos_[2],2);
    }
    center_ = {xs/ns+vertices_[0]->pos_[0], ys/ns+vertices_[0]->pos_[1], zs/ns+vertices_[0]->pos_[2]};
    return 0;
}

int Cell::getVolume() {
    double v = 0.;
    for (auto &[o, p] : polygons_) {
        if (o==-1){
            ranges::reverse(p->vertices_);
        }
        const double Rx=box_->resetDistance(p->center_[0]-center_[0],0);
        const double Ry=box_->resetDistance(p->center_[1]-center_[1],1);
        const double Rz=box_->resetDistance(p->center_[2]-center_[2],2);

        const int nv = static_cast<int>(p->vertices_.size());
        double crx = 0., cry = 0., crz = 0.;
        for (int i = 0; i < nv; i++) {
            const Vertex* vi = p->vertices_[i];
            const Vertex* vi1 = p->vertices_[(i+1)%nv];
            const double xi = box_->resetDistance(vi->pos_[0] - center_[0],0);
            const double yi = box_->resetDistance(vi->pos_[1] - center_[1],1);
            const double zi = box_->resetDistance(vi->pos_[2] - center_[2],2);
            const double xi1 = box_->resetDistance(vi1->pos_[0] - center_[0],0);
            const double yi1 = box_->resetDistance(vi1->pos_[1] - center_[1],1);
            const double zi1 = box_->resetDistance(vi1->pos_[2] - center_[2],2);
            crx += yi*zi1 - zi*yi1;
            cry += -xi*zi1 + zi*xi1;
            crz += xi*yi1 - yi*xi1;
        }
        v += Rx*crx + Ry*cry + Rz*crz;

        if (o==-1){
            ranges::reverse(p->vertices_);
        }
    }
    volume_ = v / 6;
    //return volume_;
    return 0;
}

int Cell::getArea() {
    double A = 0;
    for (auto &[o, p] : polygons_) {
        if (o==-1){
            ranges::reverse(p->vertices_);
        }
        const double Rx=box_->resetDistance(p->center_[0]-center_[0],0);
        const double Ry=box_->resetDistance(p->center_[1]-center_[1],1);
        const double Rz=box_->resetDistance(p->center_[2]-center_[2],2);

        const int nv = static_cast<int>(p->vertices_.size());
        for (int i = 0; i < nv; i++) {
            const Vertex* vi = p->vertices_[i];
            const Vertex* vl = p->vertices_[(i+1)%nv];
            const double xi = box_->resetDistance(vi->pos_[0] - center_[0],0);
            const double yi = box_->resetDistance(vi->pos_[1] - center_[1],1);
            const double zi = box_->resetDistance(vi->pos_[2] - center_[2],2);
            const double xl = box_->resetDistance(vl->pos_[0] - center_[0],0);
            const double yl = box_->resetDistance(vl->pos_[1] - center_[1],1);
            const double zl = box_->resetDistance(vl->pos_[2] - center_[2],2);
            const double Ax = yi*zl - zi*yl + Ry*(zi-zl) + Rz*(yl-yi);
            const double Ay = -(xi*zl - zi*xl + Rx*(zi-zl) + Rz*(xl-xi));
            const double Az = xi*yl - yi*xl + Rx*(yi-yl) + Ry*(xl-xi);
            A += sqrt(pow(Ax,2) + pow(Ay,2) + pow(Az,2));
        }
        if (o==-1){
            ranges::reverse(p->vertices_);
        }
    }
    area_ = A * 0.5;
    return 0;
}

int Cell::addPolygon(Polygon* p_add) {
    //p_add.is_wall_ = wall;
    int orientation = 1;
    array<double, 3> cj = {
        p_add->center_[0] - center_[0],
        p_add->center_[1] - center_[1],
        p_add->center_[2] - center_[2]
    };
    box_->resetDistanceVec(cj);

    array<double, 3> r1 = {
        p_add->vertices_[0]->pos_[0] - center_[0],
        p_add->vertices_[0]->pos_[1] - center_[1],
        p_add->vertices_[0]->pos_[2] - center_[2]
    };
    box_->resetDistanceVec(r1);
    array<double, 3> r2 = {
        p_add->vertices_[1]->pos_[0] - center_[0],
        p_add->vertices_[1]->pos_[1] - center_[1],
        p_add->vertices_[1]->pos_[2] - center_[2]
    };
    box_->resetDistanceVec(r2);

    const double r12x = r1[1]*r2[2] - r1[2]*r2[1];
    const double r12y = -r1[0]*r2[2] + r1[2]*r2[0];
    const double r12z = r1[0]*r2[1] - r1[1]*r2[0];

    if (double dp = r12x*cj[0] + r12y*cj[1] + r12z*cj[2]; dp < 0) {
        //reverse(p_add->vertices_.begin(), p_add->vertices_.end());
        orientation = -1;
    }

    cpoly cp_add{};
    cp_add.o = orientation;
    cp_add.p = p_add;

    polygons_.push_back(cp_add);
    for (Vertex* i : p_add->vertices_) {
        i->faceIds_[id_].push_back(p_add->id_);
    }
    return 0;
}

cpoly Cell::getCpoly(const int idn) {
    for (cpoly cpoly1 : polygons_) {
        if (cpoly1.p->id_ == idn) {
            return cpoly1;
        }
    }
    return cpoly{};
    //return nothing
}

int Cell::addBox(Box *box) {
    box_ = box;
    return 0;
}