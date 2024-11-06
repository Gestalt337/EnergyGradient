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

#ifndef EDGE_H_INCLUDED
#define EDGE_H_INCLUDED

class Edge;
#include "../Run/Box.h"
#include "../Vertex/Vertex.h"

class Edge {
public:
    long int id_;
    std::array<Vertex *,2> vertices_;
    array<double,3> vv_{};  // the vector pointing from vertex 0 to 1
    array<double,3> center_{};
    double length_=0.;
    bool candidate_;    // reconnection candidate edge, length shorter than Lth
    int triangle_count_;
    bool markToDelete_;
    Box* box_{};
    // 0: connected to no triangle
    // 1: connected to 1 triangle
    // 2: connected to 2 triangles
    // 3: connected to 3 triangles
    Edge(const array<Vertex*,2> vertices, const long int id): id_(id),vertices_(vertices) {
        candidate_ = false;
        triangle_count_ = 0;
        markToDelete_ = false;
    }

    bool crossBoundary() const;
    int update();
    bool checkI();
    bool checkH();
    Vertex * otherVertex(Vertex *);
    int addBox(Box*);
};

#endif
