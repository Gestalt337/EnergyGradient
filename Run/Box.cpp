//
// Created by haoyuan on 03/11/24.
//

#include "Box.h"

double Box::resetDistance(double di, int m) const {
    if (di>boxSize_[m]*0.5) {
        di-=boxSize_[m]*periodic_[m];
    }
    else if (di<-boxSize_[m]*0.5) {
        di+=boxSize_[m]*periodic_[m];
    }
    return di;
}

int Box::resetDistanceVec(std::array<double,3>& dr) const {
    for (int m=0; m<3; ++m){
        if (dr[m]>boxSize_[m]*0.5) {
            dr[m]-=boxSize_[m]*periodic_[m];
        }
        else if (dr[m]<-boxSize_[m]*0.5) {
            dr[m]+=boxSize_[m]*periodic_[m];
        }
    }
    return 0;
}
