#pragma once

#include "PolygonalMesh.hpp"

using namespace std;

namespace PolygonalMesh{

bool ImportCells0D(const string& filename, PolygonalMesh& mesh);

bool ImportCell1Ds(const string &filename,PolygonalMesh& mesh);

bool ImportCells2D (const string& filename, PolygonalMesh& mesh);

bool ImportPolygonalMesh(const string& filepath, PolygonalMesh& mesh);

double lengthEdges(const Vector2d& p1, const Vector2d& p2);

double PolygonArea(vector<Vector2d>& vertices);
}
