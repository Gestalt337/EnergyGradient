//
// Created by haoyuan on 03/11/24.
//

#ifndef BOX_H
#define BOX_H
#include <array>


class Box {
public:
    std::array<double,3> boxSize_{};
    std::array<bool,3> periodic_{};

    Box(const std::array<double,3> &boxSize, const std::array<bool,3> & periodic): boxSize_(boxSize), periodic_(periodic){}


    double resetDistance(double, int) const;
    int resetDistanceVec(std::array<double,3>&) const;
};



#endif //BOX_H
