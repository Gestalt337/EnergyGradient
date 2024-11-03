//
// Created by haoyuan on 03/11/24.
//

#ifndef VERTEX_H
#define VERTEX_H
#include <array>
#include <vector>
#include <map>

using namespace std;

class Vertex {
public:
    const int id_;
    array<double,3> pos_;
    array<double, 3> force_={0,0,0};
    array<double, 3> velocity_={0,0,0};
    map<int,vector<int>> faceIds_;

    Vertex(int id, const array<double,3> &pos) : id_(id), pos_(pos) {};
};


#endif //VERTEX_H
