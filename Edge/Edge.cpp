/* ---------------------------------------------------------------------------------
 * Copyright 2021-2023 Tao Zhang
 *
 * This file is part of TVM.
 *
 * TVM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as
 * published by the Free Software Foundation,
 * either version 3 of the License,
 * or (at your option) any later version.
 *
 * TVM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with TVM. If not, see <https://www.gnu.org/licenses/>.
 *
 * Author: Tao Zhang @ Shanghai Jiao Tong University, zhangtao.scholar@sjtu.edu.cn
 * Coauthor: Jennifer Schwarz @ Syracuse University, jschwarz@physics.syr.edu
 * ---------------------------------------------------------------------------------
 */

#include <cstdio>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <chrono>
#include <unordered_map>

#include "Edge.h"

using namespace std;

bool Edge::crossBoundary() const {
    if (fabs(vertices_[1]->pos_[0] - vertices_[0]->pos_[0]) > box_->boxSize_[0]/2.0) {
        return true;
    }
    if (fabs(vertices_[1]->pos_[1] - vertices_[0]->pos_[1]) > box_->boxSize_[1]/2.0) {
        return true;
    }
    if (fabs(vertices_[1]->pos_[2] - vertices_[0]->pos_[2]) > box_->boxSize_[2]/2.0) {
        return true;
    }

    return false;
}

int Edge::update() {
//    cout << vertices_.size() << endl;
    vv_[0] = vertices_[1]->pos_[0] - vertices_[0]->pos_[0];
    vv_[1] = vertices_[1]->pos_[1] - vertices_[0]->pos_[1];
    vv_[2] = vertices_[1]->pos_[2] - vertices_[0]->pos_[2];
    box_->resetDistanceVec(vv_);
    length_ = sqrt(vv_[0]*vv_[0] + vv_[1]*vv_[1] + vv_[2]*vv_[2]);
    center_[0] = vertices_[0]->pos_[0] + vv_[0]/2.0;
    center_[1] = vertices_[0]->pos_[1] + vv_[1]/2.0;
    center_[2] = vertices_[0]->pos_[2] + vv_[2]/2.0;

    return 0;
}

bool Edge::checkI() {
    if (!candidate_) {
        return false;
    }
    if (triangle_count_ > 0) {
        return false;
    }

    return true;
}

bool Edge::checkH() {
    if (!candidate_) {
        return false;
    }
    if (triangle_count_ != 1) {
        return false;
    }

    return true;
}

Vertex * Edge::otherVertex(Vertex * v0) {
    if (v0 == vertices_[0]) {
        return vertices_[1];
    } else if (v0 == vertices_[1]) {
        return vertices_[0];
    } else {
        return NULL;
    }
}

int Edge::addBox(Box *box) {
    box_ = box;
    return 0;
}