// AUTHOR: CHUN KIT CALVIN LI
// DATE:   AUGUST 22th, 2022
// TITLE:  GMSH_AIRFOIL_READER

#include <iostream>
#include <gmsh.h>
#include "read_data.h"

FILE *pt1;
char *filename1;                                    //Output Mesh file

using namespace std;

int main(int argc, char const *argv[]) {

    //Assert that input argument is not empty
    M_Assert(argv[1] != 0, "ERROR# : Input argument is empty. Please insert an input file.");

    //READ AIRFOIL DATA POINTS TO VECTOR PAIR
    vector<pair<double, double> > airfoil_data;

    cout << "------------------------  GMSH AIRFOIL ------------------------" << endl;
    cout << endl;

//    //GET OUTPUT FILE READY
//    filename1 = "airfoil.msh";                                            //DATA FOR GMSH AIRFOIL SPLINE
//    pt1 = fopen(filename1, "r");

    int size = fileReader(argv[1], airfoil_data);

    std::cout << "Size of Airfoil #Points = " << size << std::endl;

//    printVectorPair(airfoil_data);

    /**
     * STARTING TO USE GMSH API HERE FOR FUN AND TEST
     */
    gmsh::initialize();
    gmsh::model::add("SD7003");

    vector<pair<int, double>> offset_line;

    vector<int> spline_tag;
    vector<int> spline_tag2;
    vector<int> spline_tag3;

    int p_physical_tag = 0;
    double spline_meshsize = 5;

    //NEED TO MAKE SURE THAT AIRFOIL DATA STARTS @ 1
    for (int i = 0; i < (size - 1)/2; i++) {
        p_physical_tag++;
        spline_tag.push_back(p_physical_tag);
        gmsh::model::geo::addPoint(airfoil_data[i].first,airfoil_data[i].second, 0.0, spline_meshsize, p_physical_tag);
    }

    spline_tag2.push_back(p_physical_tag);

    //NEED TO MAKE SURE THAT AIRFOIL DATA STARTS @ 1
    for (int i = (size - 1)/2; i < size - 1; i++) {
        p_physical_tag++;
        spline_tag2.push_back(p_physical_tag);
        gmsh::model::geo::addPoint(airfoil_data[i].first,airfoil_data[i].second, 0.0, spline_meshsize, p_physical_tag);
    }

    spline_tag2.push_back(1);
    cout << "Size of spline_tag = " << spline_tag.size() << endl;
    cout << "Size of spline_tag2 = " << spline_tag2.size() << endl;

    gmsh::model::geo::addSpline(spline_tag, 1);
    gmsh::model::geo::addSpline(spline_tag2, 2);

    double meshsize = 10;

    double box_x_min = -2;
    double box_x_max = 3;
    double box_y_min = -2;
    double box_y_max = 2;

    gmsh::model::geo::addPoint(1,10,0, meshsize, 100);
    gmsh::model::geo::addPoint(12,10,0, meshsize, 101);
    gmsh::model::geo::addPoint(12,-10,0, meshsize, 102);
    gmsh::model::geo::addPoint(1,-10,0, meshsize, 103);

    gmsh::model::geo::addCircleArc(100,1,103, 10);

//    gmsh::model::geo::addPoint(box_x_min,box_y_max,0, meshsize, 100);
//    gmsh::model::geo::addPoint(box_x_max,box_y_max,0, meshsize, 101);
//    gmsh::model::geo::addPoint(box_x_max,box_y_min,0, meshsize, 102);
//    gmsh::model::geo::addPoint(box_x_min,box_y_min,0, meshsize, 103);

    gmsh::model::geo::addLine(101,100,100);
//    gmsh::model::geo::addLine(100,103,101);
    gmsh::model::geo::addLine(103,102,102);
    gmsh::model::geo::addLine(102,101,103);

    gmsh::model::geo::addCurveLoop({1,2},1);
    gmsh::model::geo::addCurveLoop({100,10,102,103},2);

    //Transfinite Curve
    gmsh::model::geo::mesh::setTransfiniteCurve(1, 100);
    gmsh::model::geo::mesh::setTransfiniteCurve(2, 100);

    gmsh::model::geo::addPlaneSurface({2,1}, 1);

    gmsh::model::geo::addPoint(1.5,0,0,spline_meshsize,500);
    gmsh::model::geo::addLine(1,500, 500);

    gmsh::model::geo::synchronize();

    gmsh::model::mesh::embed(1,{500}, 2, {1});
    gmsh::model::geo::mesh::setTransfiniteCurve(500, 10);

    gmsh::model::geo::synchronize();

    gmsh::model::mesh::field::add("BoundaryLayer",1);
    gmsh::model::mesh::field::setNumbers(1, "CurvesList", {1,2, 500});
    gmsh::model::mesh::field::setNumber(1, "SizeFar", 0.1);
    gmsh::model::mesh::field::setNumber(1, "Size", 0.0005);
    gmsh::model::mesh::field::setNumber(1, "Ratio", 1.5);
    gmsh::model::mesh::field::setNumber(1, "Thickness", 0.03);
    gmsh::model::mesh::field::setNumbers(1, "FanPointsList", {500});
//    gmsh::model::mesh::field::setNumber(1, "Quads", 1);
    gmsh::model::mesh::field::setNumber(1,"NbLayers", 10);
    gmsh::model::mesh::field::setAsBoundaryLayer(1);

    gmsh::model::geo::synchronize();

    gmsh::model::mesh::field::add("Box", 2);
    gmsh::model::mesh::field::setNumber(2,"VIn",0.2);
    gmsh::model::mesh::field::setNumber(2,"VOut",2);
    gmsh::model::mesh::field::setNumber(2,"XMin",-2);
    gmsh::model::mesh::field::setNumber(2,"XMax",3);
    gmsh::model::mesh::field::setNumber(2,"YMin",-2);
    gmsh::model::mesh::field::setNumber(2,"YMax",2);
    gmsh::model::mesh::field::setNumber(2,"Thickness",2);

    gmsh::model::geo::synchronize();
//    gmsh::model::mesh::field::setAsBackgroundMesh(2);

    gmsh::model::mesh::field::add("MinAniso", 3);
    gmsh::model::mesh::field::setNumbers(3,"FieldsList",{1,2});
    gmsh::model::mesh::field::setAsBackgroundMesh(3);

    gmsh::model::geo::synchronize();

//    gmsh::option::setNumber("Mesh.Algorithm", 2);
    gmsh::option::setNumber("Mesh.RecombinationAlgorithm", 1); // or
    gmsh::option::setNumber("Mesh.RecombineAll", 1);
    gmsh::option::setNumber("Mesh.Smoothing", 100);

    double h = 0.2;
    double dh = 0.1/(6);
    std::vector<std::pair<int, int> > ov;
//    gmsh::model::geo::extrude({{2, 1}}, 0, 0, h, ov, {1, 1, 1, 1, 1, 1}, {dh, 2*dh, 3*dh, 4*dh, 5*dh, 6*dh}, true);

    gmsh::model::geo::extrude({{2, 1}}, 0, 0, h, ov, {1}, {1}, true);

    gmsh::model::geo::synchronize();
//    gmsh::model::mesh::generate(2);

//    gmsh::option::setNumber("Mesh.MeshSizeExtendFromBoundary", 10);
//    gmsh::option::setNumber("Mesh.MeshSizeFromPoints", 10);
//    gmsh::option::setNumber("Mesh.MeshSizeFromCurvature", 10);

    gmsh::model::mesh::generate(3);
//    gmsh::model::mesh::refine();

    gmsh::model::mesh::setOrder(2);
    gmsh::model::mesh::optimize("HighOrderFastCurving");
//    gmsh::model::mesh::optimize("HighOrder");
//    gmsh::model::mesh::optimize("HighOrderElastic");
    gmsh::model::mesh::setRecombine(2,1);

    gmsh::write("SD7003.msh");
    gmsh::write("SD7003.geo_unrolled");
    gmsh::finalize();

    cout << endl;
    cout << "------------------------ END GMSH AIRFOIL ------------------------" << endl;

    return 0;
}
