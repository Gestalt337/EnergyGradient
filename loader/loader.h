//
// Created by haoyuan on 03/11/24.
//

#ifndef LOADER_H
#define LOADER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <array>
using namespace std;

map<array<double,3>,array<bool,3>> loadBox(const string&);
map<string, double> loadParam(const string&);
#endif //LOADER_H
