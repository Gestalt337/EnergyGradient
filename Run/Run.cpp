//
// Created by haoyuan on 03/11/24.
//

#include "Run.h"

int Run::computeForce(){
    // Reset forces
    for (Vertex &vtx:vertices_){
        vtx.force_={0.,0.,0.};
    }
    for (Cell& cl:cells_){
        double v = cl.volume_;
        double P = 2.*kv_*(v-v0_);
        double a = cl.area_;
        double T = 2.*ks_*(a-s0_);
        cl.energy_ = pow(kv_*(v-v0_),2) + pow(ks_*(a-s0_),2);
        int lenv =static_cast<int>(cl.vertices_.size());
        for (Vertex* vtx:cl.vertices_){
            double dvdx=0., dvdy=0., dvdz=0.;
            double dadx=0., dady=0., dadz=0.;
            for (cpoly& polystr:cl.polygons_){
                if (polystr.o==-1){
                    ranges::reverse(polystr.p->vertices_);
                }
                Polygon* poly = polystr.p;
                double Rx = box_->resetDistance(poly->center_[0]-cl.center_[0],0);
                double Ry = box_->resetDistance(poly->center_[1]-cl.center_[1],1);
                double Rz = box_->resetDistance(poly->center_[2]-cl.center_[2],2);
                int lenp = static_cast<int>(size(poly->vertices_));
                double M= 1./lenp - 1./lenv;

                for (int i=0;i<lenp;++i){
                    double xim1 = poly->vertices_[(i-1+lenp)%lenp]->pos_[0];
                    double yim1 = poly->vertices_[(i-1+lenp)%lenp]->pos_[1];
                    double zim1 = poly->vertices_[(i-1+lenp)%lenp]->pos_[2];
                    double xi = poly->vertices_[i]->pos_[0];
                    double yi = poly->vertices_[i]->pos_[1];
                    double zi = poly->vertices_[i]->pos_[2];
                    double xip1 = poly->vertices_[(i+1)%lenp]->pos_[0];
                    double yip1 = poly->vertices_[(i+1)%lenp]->pos_[1];
                    double zip1 = poly->vertices_[(i+1)%lenp]->pos_[2];
                    double xip = box_->resetDistance(poly->vertices_[i]->pos_[0]-cl.center_[0],0);
                    double yip = box_->resetDistance(poly->vertices_[i]->pos_[1]-cl.center_[1],1);
                    double zip = box_->resetDistance(poly->vertices_[i]->pos_[2]-cl.center_[2],2);
                    double xip1p = box_->resetDistance(poly->vertices_[(i+1)%lenp]->pos_[0]-cl.center_[0],0);
                    double yip1p = box_->resetDistance(poly->vertices_[(i+1)%lenp]->pos_[1]-cl.center_[1],1);
                    double zip1p = box_->resetDistance(poly->vertices_[(i+1)%lenp]->pos_[2]-cl.center_[2],2);
                    // p and pm are essentially short for prime. But they are subtracted by different lengths.
                    double xipm = box_->resetDistance(poly->vertices_[i]->pos_[0]-poly->center_[0],0);
                    double yipm = box_->resetDistance(poly->vertices_[i]->pos_[1]-poly->center_[1],1);
                    double zipm = box_->resetDistance(poly->vertices_[i]->pos_[2]-poly->center_[2],2);
                    double xip1pm = box_->resetDistance(poly->vertices_[(i+1)%lenp]->pos_[0]-poly->center_[0],0);
                    double yip1pm = box_->resetDistance(poly->vertices_[(i+1)%lenp]->pos_[1]-poly->center_[1],1);
                    double zip1pm = box_->resetDistance(poly->vertices_[(i+1)%lenp]->pos_[2]-poly->center_[2],2);

                    double Si=pow((yipm*zip1pm-zipm*yip1pm),2)+pow((xipm*zip1pm-zipm*xip1pm),2)+pow((xipm*yip1pm-yipm*xip1pm),2);

                    if(find(vtx->faceIds_[cl.id_].begin(), vtx->faceIds_[cl.id_].end(), poly->id_) != vtx->faceIds_[cl.id_].end()){
                        if (poly->vertices_[i]==vtx){
                            dvdx+=Ry*box_->resetDistance((zim1-zip1),2)+Rz*box_->resetDistance((yip1-yim1),1);
                            dvdy-=Rx*box_->resetDistance((zim1-zip1),2)+Rz*box_->resetDistance((xip1-xim1),0);
                            dvdz+=Rx*box_->resetDistance((yim1-yip1),1)+Ry*box_->resetDistance((xip1-xim1),0);

                            dadx+=1./sqrt(Si) * (xipm*(pow(yip1pm,2)+pow(zip1pm,2))-xip1pm*(yipm*yip1pm+zipm*zip1pm));
                            dady+=1./sqrt(Si) * (yipm*(pow(zip1pm,2)+pow(xip1pm,2))-yip1pm*(zipm*zip1pm+xipm*xip1pm));
                            dadz+=1./sqrt(Si) * (zipm*(pow(xip1pm,2)+pow(yip1pm,2))-zip1pm*(xipm*xip1pm+yipm*yip1pm));
                        }
                        else if (poly->vertices_[(i+1)%lenp]==vtx){

                            dadx+=1./sqrt(Si) * (xip1pm*(pow(yipm,2)+pow(zipm,2))-xipm*(yipm*yip1pm+zipm*zip1pm));
                            dady+=1./sqrt(Si) * (yip1pm*(pow(zipm,2)+pow(xipm,2))-yipm*(zipm*zip1pm+xipm*xip1pm));
                            dadz+=1./sqrt(Si) * (zip1pm*(pow(xipm,2)+pow(yipm,2))-zipm*(xipm*xip1pm+yipm*yip1pm));
                        }
                    }
                    dvdx+=M*(yip*zip1p-zip*yip1p)+1./lenv * (Rz*box_->resetDistance((yi-yip1),1)-Ry*box_->resetDistance((zi-zip1),2));
                    dvdy+=M*(zip*xip1p-xip*zip1p)+1./lenv * (Rx*box_->resetDistance((zi-zip1),2)-Rz*box_->resetDistance((xi-xip1),0));
                    dvdz+=M*(xip*yip1p-yip*xip1p)+1./lenv * (Ry*box_->resetDistance((xi-xip1),0)-Rx*box_->resetDistance((yi-yip1),1));

                    dadx-=(1./lenp)*(xipm*(pow(yip1pm,2)+pow(zip1pm,2))-(xipm+xip1pm)*(yipm*yip1pm+zipm*zip1pm)+xip1pm*(pow(yipm,2)+pow(zipm,2)));
                    dady-=(1./lenp)*(yipm*(pow(zip1pm,2)+pow(xip1pm,2))-(yipm+yip1pm)*(zipm*zip1pm+xipm*xip1pm)+yip1pm*(pow(zipm,2)+pow(xipm,2)));
                    dadz-=(1./lenp)*(zipm*(pow(xip1pm,2)+pow(yip1pm,2))-(zipm+zip1pm)*(xipm*xip1pm+yipm*yip1pm)+zip1pm*(pow(xipm,2)+pow(yipm,2)));
                }
                if (polystr.o==-1){
                    ranges::reverse(polystr.p->vertices_);
                }
            }
            vtx->force_[0] +=-T*dadx*0.5-P*dvdx*1./6;
            vtx->force_[1] +=-T*dady*0.5-P*dvdy*1./6;
            vtx->force_[2] +=-T*dadz*0.5-P*dvdz*1./6;
        }
    }
    return 0;
}

int Run::updateEnergy() {
    double E = 0;
    for (Cell& cell:cells_){
        E += cell.energy_;
    }
    energy_ = E;
    return 0;
}

int Run::getVertexVelocity() {
    for (Vertex& vertex : vertices_) {
        for (int m = 0; m < 3; ++m) {
            vertex.velocity_[m] = mu_ * vertex.force_[m];
        }
    }
    // remove drift velocity
    double average_velo[3] = {0., 0., 0.};
    for (Vertex& vertex : vertices_) {
        for (int m = 0; m < 3; ++m) {
            average_velo[m] += vertex.velocity_[m];
        }
    }
    for (double & velo : average_velo) {
        velo /= static_cast<double>(vertices_.size());
    }
    for (Vertex& vertex : vertices_) {
        for (int m = 0; m < 3; m++) {
            vertex.velocity_[m] = vertex.velocity_[m] - average_velo[m];
        }
    }
    return 0;
}

int Run::updateCell() {
    ITERS_ += 1;
    for (Vertex & vertex : vertices_) {
        for (int m = 0; m < 3; m++) {
            vertex.pos_[m] += vertex.velocity_[m]*dt_;
            if (vertex.pos_[m]>box_->boxSize_[m] && box_->periodic_[m]) {
                vertex.pos_[m] -= box_->boxSize_[m];
            }
            else if (vertex.pos_[m]<0 && box_->periodic_[m]) {
                vertex.pos_[m] += box_->boxSize_[m];
            }
        }
    }
    // Recalculate cell geometric.
    for (Cell &cell:cells_){
        cell.getCenter();
        cell.getVolume();
        cell.getArea();
        for (cpoly& poly:cell.polygons_){
            poly.p->getCenter();
        }
    }
    return 0;
}

int Run::addBox(Box *box) {
    box_ = box;
    for (Cell& cell: cells_) {
        cell.addBox(box);
    }
    for (Polygon& poly: polygons_) {
        poly.addBox(box);
    }
    for (Edge& edge: edges_) {
        edge.addBox(box);
    }
    return 0;
}

int Run::computeCellCenters() {
    for (Cell& cell: cells_) {
        cell.getCenter();
    }
    return 0;
}

int Run::computePolygonCenters() {
    for (Polygon& poly: polygons_) {
        poly.getCenter();
    }
    return 0;
}

Vertex* Run::getVertexPtr(int i) {
    for (Vertex& v:vertices_) {
        if (v.id_ == i) {
            return &v;
        }
    }
    return nullptr;
}

Polygon* Run::getPolygonPtr(int k) {
    for (Polygon& p:polygons_) {
        if (p.id_ == k) {
            return &p;
        }
    }
    return nullptr;
}

Cell* Run::getCellPtr(int j) {
    for (Cell& c:cells_) {
        if (c.id_ == j) {
            return &c;
        }
    }
    return nullptr;
}

int Run::addCells(){

    ifstream file("../data/cell.csv");
    string line;
    int cell_id;

    while (std::getline(file, line)) {

        std::vector<std::string> row;   // Vector to store the split values for each row
        std::stringstream ss(line);      // Create a string stream from the line
        std::string value;

        // Split the line by commas
        while (std::getline(ss, value, ',')) {
            row.push_back(value);         // Add each value to the row vector
        }
        if (row[0]=="Cell"){cell_id=stoi(row[1]);}
        else{
            vector<Vertex*> vts={};
            for(auto i:row){vts.push_back(getVertexPtr(stoi(i)));}
            cells_.push_back(Cell(cell_id,vts));
        }
    }
    return 0;
}

int Run::addVertices() {
    ifstream file("../data/vertices.csv");
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string value;
        vector<string> row;
        while (getline(ss, value, ',')) {
            row.push_back(value);
        }
        int id = stoi(row[0]);
        array<double, 3> pos{stod(row[1]), stod(row[2]), stod(row[3])};
        vertices_.emplace_back(id, pos);
    }
    return 0;
}

int Run::addEdges() {
    ifstream file("../data/edges.csv");
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string value;
        vector<string> row;
        while (getline(ss, value, ',')) {
            row.push_back(value);
        }
        const long int id = stol(row[0]);
        auto vtx1 = getVertexPtr(1);
        edges_.push_back(Edge({getVertexPtr(stoi(row[1])),getVertexPtr(stoi(row[2]))},id));
    }
    return 0;
}

int Run::addPolygonRun() {
    ifstream file("../data/polygons.csv");
    string line;

    while (getline(file, line)) {

        vector<std::string> row;   // Vector to store the split values for each row
        stringstream ss(line);      // Create a string stream from the line
        string value;

        // Split the line by commas
        while (getline(ss, value, ',')) {
            row.push_back(value);         // Add each value to the row vector
        }
        int poly_id = stoi(row[0]);
        vector<Vertex*> poly_v={};
        for (int i=1;i<row.size();++i){
            poly_v.push_back(getVertexPtr(stoi(row[i])));
        }
        polygons_.push_back(Polygon(poly_id,poly_v));
    }
    return 0;
}

int Run::addPolygonCell() {
    ifstream file("../data/polygon.csv");
    string line;
    int cell_id=-1;
    Cell* thecell=nullptr;

    while (getline(file, line)) {

        vector<std::string> row;   // Vector to store the split values for each row
        stringstream ss(line);      // Create a string stream from the line
        string value;

        // Split the line by commas
        while (getline(ss, value, ',')) {
            row.push_back(value);         // Add each value to the row vector
        }
        if (row[0]=="Cell"){
            cell_id=stoi(row[1]);
            thecell = getCellPtr(cell_id);
        }
        else{
            for (const string& pid:row){
                Polygon* p_add = getPolygonPtr(stoi(pid));
                int np = static_cast<int>(p_add->vertices_.size());
                int orientation;
                vector<cedge> cedges{};
                for (int i=0; i<np; ++i) {
                    for (Edge& edge:edges_) {
                        if (edge.vertices_[0]==p_add->vertices_[i] && edge.vertices_[1]==p_add->vertices_[(i+1)%np]) {
                            cedges.push_back(cedge(1,&edge));
                            break;
                        }
                        else if (edge.vertices_[0]==p_add->vertices_[(i+1)%np] && edge.vertices_[1]==p_add->vertices_[i]) {
                            cedges.push_back(cedge(-1,&edge));
                            break;
                        }
                    }
                }
                p_add->edges_ = std::move(cedges);
                for (Vertex* v:p_add->vertices_) {
                }
                thecell->addPolygon(p_add);
                p_add->cellIds_.push_back(cell_id);
            }
            //thecell->get_volume();
            //thecell->get_area();
        }
    }
    return 0;
}

int Run::updateEdges() {
    for (Edge& edge:edges_) {
        edge.update();
    }
    return 0;
}


int Run::computeCellVA() {
    for (Cell& cell: cells_) {
        cell.getVolume();
        cell.getArea();
    }
    return 0;
}
