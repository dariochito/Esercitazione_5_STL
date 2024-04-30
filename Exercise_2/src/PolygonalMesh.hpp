#pragma once
#include "Eigen\Eigen"

using namespace std;
using namespace Eigen;

namespace PolygonalMesh{

struct PolygonalMesh{

    unsigned int NumberCell0D = 0; //Number of Cells 0D.
    vector<unsigned int> Cell0DId = {}; //(Dynamical) Vector containing the Ids of the Cells 0D. Its size is egual to NumberCell0D.
    vector<Vector2d> Cell0DCoordinates = {}; //(Dynamical) Vector whose elemets are 2D vectors containg the coordinates of each Cell0D.
    map<unsigned int, list<unsigned int>> Cell0DMarkers = {}; //Map that assigns to each marker (different from zero) a list of Cells 0D with that same marker.

    unsigned int NumberCell1D = 0; //Number of Cells 1D;
    vector<unsigned int> Cell1DId = {}; //(Dynamical) Vector containing the Ids of the Cells 1D. Its size is egual to NumberCell1D.
    vector<Vector2i> Cell1DVertices = {}; //(Dynamical) Vector whose elemets are 2D vectors containg the Ids of the origin and end of the Cells 0D defining the Cell 1D.
    map<unsigned int, list<unsigned int>> Cell1DMarkers = {}; //Map that assigns to each marker (different from zero) a list of Cells 1D with that same marker.

    unsigned int NumberCell2D = 0; //Number of Cells 2D;
    vector<unsigned int> Cell2DId = {}; //(Dynamical) Vector containing the Ids of the Cells 2D. Its size is egual to NumberCell2D.
    vector<unsigned int> NumberVertices = {}; //Number of vertices of each Cell 2D.
    vector<unsigned int> NumberEdges = {}; //Number of edges of each Cell 2D.
    vector<vector<unsigned int>> Cell2DVertices = {}; //(Dynamical) Vector whose elemets are dynamical vectors as well containg the Ids of the NumberVertices vertices of each Cell 2D.
    vector<vector<unsigned int>> Cell2DEdges = {}; //(Dynamical) Vector whose elemets are dynamical vectors as well containg the Ids of the NumberEdges edges of each Cell 2D.
};
}

