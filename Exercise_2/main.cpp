#include "PolygonalMesh.hpp"
#include "Utils.hpp"
#include <iostream>

using namespace std;

int main()
{
     string filename = "PolygonalMesh";
     PolygonalMesh::PolygonalMesh mesh;
     if (PolygonalMesh::ImportPolygonalMesh(filename,mesh) == true)
     {
         cout << "Mesh imported successfully." << endl;
     }
     else cout << "Mesh NOT imported successfully." << endl;

     return 0;
}
