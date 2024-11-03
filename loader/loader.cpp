//
// Created by haoyuan on 03/11/24.
//

#include "loader.h"
#include "../Run/Run.h"

map<array<double,3>,array<bool,3>> loadBox(const string& fname) {
    map<array<double,3>,array<bool,3>> map{};
    std::ifstream file(fname);
    std::string line;
    array<double,3> boxSize{};
    array<bool,3> periodic{};
    // Read each line from the file
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string entry;
        std::vector<std::string> row;
        // Split each line by comma
        while (std::getline(ss, entry, ',')) {
            row.push_back(entry);
        }

        if (row[0]=="box") {
            boxSize[0]=std::stod(row[1]);
            boxSize[1]=std::stod(row[2]);
            boxSize[2]=std::stod(row[3]);
            periodic[0]=static_cast<bool>(std::stoi(row[4]));
            periodic[1]=static_cast<bool>(std::stoi(row[5]));
            periodic[2]=static_cast<bool>(std::stoi(row[6]));
        }
    }

    file.close();
    map[boxSize]=periodic;
    return map;
}

map<string, double> loadParam(const string& fname) {
    map<string, double> vars;
    ifstream file(fname);
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string value;
        vector<string> row;

        // Parse each line by commas
        while (getline(ss, value, ',')) {
            row.push_back(value);
        }

        // Ensure row has at least 2 elements before accessing row[1]
        if (row.size() > 1) {
            if (row[0] == "dt") {
                vars["dt"] = stod(row[1]);
            }
            // Ensure row has at least 3 elements before accessing row[2]
            else if (row[0] == "kv") {
                vars["kv"] = stod(row[1]);
            }
            else if (row[0] == "v0") {
                vars["v0"] = stod(row[1]);
            }
            else if (row[0] == "s0") {
                vars["s0"] = stod(row[1]);
            }
            // else if (row[0] == "mu") {
            //     vars["mu"] = stod(row[1]);
            // }
        } else {
            cerr << "Warning: Line doesn't have enough values: " << line << endl;
        }
    }
    return vars;
}