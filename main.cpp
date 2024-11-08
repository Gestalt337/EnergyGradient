#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <filesystem>

#include "Run/Box.h"
#include "Polygon/Polygon.h"
#include "loader/loader.h"
#include "Run/Run.h"

using namespace std;

void dumpVtk(Run& run){
    int v_num = 0;
    int p_num = 0;
    string wrt;
    // Checking corss boundary polygons
    // for (Polygon& p:run.polygons_) {
    //     if (run.box_->periodic_[0]||run.box_->periodic_[1]||run.box_->periodic_[2]) {
    //         p.checkBoundary();
    //     }
    // }
    wrt.append("# vtk DataFile Version 3.0\npolydata\nASCII\nDATASET POLYDATA\n");
    wrt.append("POINTS " + to_string(run.vertices_.size())+" double\n");
    for(Vertex& v: run.vertices_){
        wrt.append(to_string(v.pos_[0])+" "+to_string(v.pos_[1])+" "+to_string(v.pos_[2])+"\n");
    }
    wrt.append("\n");
    string to_find = "PLACEHOLDER";
    wrt.append(to_find);
    //s+="POLYGONS "+to_string(p_num)+" "+to_string(p_num+v_num)+"\n";//discarding any cross boundary polygon for better visualization.
        //p_num+=static_cast<int>(c.polygons_.size());
    for(Polygon& p:run.polygons_){
        if (!p.checkBoundary()) {
            p_num +=1;
            wrt.append(to_string(p.vertices_.size())+" ");
            for(Vertex* v:p.vertices_){
                wrt.append(to_string(v->id_)+" ");
                v_num+=1;
            }
            wrt.append("\n");
        }
    }
    size_t pos = wrt.find(to_find);
    wrt.replace(pos, to_find.size(), "POLYGONS "+to_string(p_num)+" "+to_string(p_num+v_num)+"\n");
    ofstream outFile("../data/output/vtk/topo_"+to_string(run.ITERS_)+".vtk");
    outFile<<wrt;
    outFile.close();
}

int main()//Note: For this configuration, z-force on monolayer is set to 0.
{
    long MAX_ITER = 100000;
    int SAVE_ITER = 200;
    map<array<double,3>,array<bool,3>> boxMap = loadBox("../conf");
    auto it = boxMap.begin();
    Box simBox(it->first,it->second);
    map<string, double> param = loadParam("../conf");
    double kv=param["kv"], v0=param["v0"], s0=param["s0"], dt=param["dt"];
    auto container=Run(kv, v0, s0, dt);
    container.addBox(&simBox);
    container.updateEdges();
    container.computeCellCenters();
    container.computePolygonCenters();
    container.addPolygonCell();
    container.computeCellVA();
    container.computeForce();
    container.updateEnergy();
    container.getVertexVelocity();

    ofstream volumeFile("data/output/volume.txt");
    ofstream areaFile("data/output/area.txt");
    ofstream forceFile("data/output/force.txt");
    volumeFile<<"ITER "+to_string(container.ITERS_)<<endl;
    areaFile<<"ITER "+to_string(container.ITERS_)<<endl;
    forceFile<<"ITER "+to_string(container.ITERS_)<<endl;
    for (Vertex vertex:container.vertices_){
        forceFile<<vertex.id_<<" "<<sqrt(pow(vertex.force_[0],2)+pow(vertex.force_[1],2)+pow(vertex.force_[2],2))<<endl;
    }
    for (auto & cell : container.cells_){
        volumeFile<<"Cell"+to_string(cell.id_)+" "+to_string(cell.volume_)<<endl;
        areaFile<<"Cell"+to_string(cell.id_)+" "+to_string(cell.area_)<<endl;
    }

    filesystem::remove_all("../data/output/vtk/");
    filesystem::create_directories("../data/output/vtk/");
    cout<<">>>>>>Simulation Starts>>>>>>"<<endl;
    cout<<"ITER "<<container.ITERS_<<endl;
    cout<< "Current Energy "<<container.energy_<<endl;
    dumpVtk(container);
    container.updateCell();
    container.updateEdges();
    while (container.ITERS_<=MAX_ITER) {
        container.computeForce();
        container.updateEnergy();
        container.getVertexVelocity();
        if (container.ITERS_%SAVE_ITER==0) {
            cout<<"ITER "<<container.ITERS_<<endl;
            cout<< "Current Energy "<<container.energy_<<endl;
            volumeFile<<"ITER "+to_string(container.ITERS_)<<endl;
            areaFile<<"ITER "+to_string(container.ITERS_)<<endl;
            forceFile<<"ITER "+to_string(container.ITERS_)<<endl;
            for (Vertex vertex:container.vertices_){
                forceFile<<vertex.id_<<" "<<sqrt(pow(vertex.force_[0],2)+pow(vertex.force_[1],2)+pow(vertex.force_[2],2))<<endl;
            }
            for (auto & cell : container.cells_){
                volumeFile<<"Cell"+to_string(cell.id_)+" "+to_string(cell.volume_)<<endl;
                areaFile<<"Cell"+to_string(cell.id_)+" "+to_string(cell.area_)<<endl;
            }
            dumpVtk(container);
        }
        container.updateCell();
        container.updateEdges();
    }
    volumeFile.close();
    areaFile.close();
    forceFile.close();

    return 0;
}
