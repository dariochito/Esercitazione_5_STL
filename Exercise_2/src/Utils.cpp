#include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <limits>

using namespace std;

namespace PolygonalMesh{

//Function that computes the length of the edges.
double lengthEdges(const Vector2d& p1, const Vector2d& p2)
{
    double length = sqrt(abs(p1(0) - p2(0)) + abs(p1(1) - p2(1)));
    return length;
}

/**Function that computes the area of the polygons:
 * it takes as imput a vector whose elemets are the coordinates of the Cells0D that form
   each edge describing the polygon, and then it uses Gauss formula to calculate its area */
double PolygonArea(vector<Vector2d>& vertices)
{
    double area = 0;
    unsigned int n = vertices.size();
    for(unsigned int i = 0; i < n; i++)
    {
        unsigned int j = (i + 1) % n;
        area += vertices[i][0] * vertices[j][1] - vertices[j][0] * vertices[i][1];
    }
    return abs(area)/2;
}

bool ImportPolygonalMesh(const string& filepath, PolygonalMesh& mesh)
{
    if(!ImportCells0D(filepath + "/Cell0Ds.csv",mesh))
    {
        return false;
    }
    else
    {
        cout << "Cell0D marker:" << endl;
        for(auto it = mesh.Cell0DMarkers.begin(); it != mesh.Cell0DMarkers.end(); it++)
        {
            cout << "key:\t" << it -> first << "\t values:";
            for(const unsigned int id : it -> second)
                cout << "\t" << id;
            cout << endl;
        }
    }

    if(!ImportCell1Ds(filepath + "/Cell1Ds.csv",
                       mesh))
    {
        return false;
    }
    else
    {
        cout << "Cell1D marker:" << endl;
        for(auto it = mesh.Cell1DMarkers.begin(); it != mesh.Cell1DMarkers.end(); it++)
        {
            cout << "key:\t" << it -> first << "\t values:";
            for(const unsigned int id : it -> second)
                cout << "\t" << id;

            cout << endl;
        }
    }

    if(!ImportCells2D(filepath + "/Cell2Ds.csv",mesh))
    {
        return false;
    }
    else
    {
        //TEST_1
        for(unsigned int c = 0; c < mesh.NumberCell2D; c++)
        {
            vector<unsigned int> edges = mesh.Cell2DEdges[c]; //Edges ids of the c-th 2D cell.

            for(unsigned int e = 0; e < mesh.NumberEdges[c]; e++)
            {
                const unsigned int origin = mesh.Cell1DVertices[edges[e]][0]; // id of the origin point of the edge.
                const unsigned int end = mesh.Cell1DVertices[edges[e]][1]; //id of the end point of the edge.

                auto findOrigin = find(mesh.Cell2DVertices[c].begin(), mesh.Cell2DVertices[c].end(), origin);
                if(findOrigin == mesh.Cell2DVertices[c].end())
                {
                    cerr << "Wrong mesh" << endl;
                    return 2;
                }

                auto findEnd = find(mesh.Cell2DVertices[c].begin(), mesh.Cell2DVertices[c].end(), end);
                if(findEnd == mesh.Cell2DVertices[c].end())
                {
                    cerr << "Wrong mesh" << endl;
                    return 3;
                }

            }
        }

        //TEST_2: we check if all edges have non zero length.
        for(unsigned int c = 0; c < mesh.NumberCell2D; c++)
        {
            vector<unsigned int> edges = mesh.Cell2DEdges[c]; //Vector containing the Cells 1D ids of the selected Cell 2D.
            for (unsigned int e = 0; e < mesh.NumberEdges[c] ; e++)
            {
                unsigned int origin = mesh.Cell1DVertices[edges[e]][0]; //Id of the origin of the Cell 1D edges[e].
                unsigned int end = mesh.Cell1DVertices[edges[e]][1]; //Id of the origin of the Cell 1D edges[e].
                Vector2d origin_coordinates = mesh.Cell0DCoordinates[origin]; //Coordinates of the origin point of the edge.
                Vector2d end_coordinates = mesh.Cell0DCoordinates[end]; //Coordinates of the end point of the edge.
                if(lengthEdges(origin_coordinates,end_coordinates) == 0)
                    cerr << "Wrong mesh" << endl;
            }
        }

        //TEST_3: we check if all polygons have non zero area.
        for(unsigned int c = 0; c < mesh.NumberCell2D; c++)
        {
            vector<unsigned int> vertices = mesh.Cell2DVertices[c]; //Vector containing the Cells 0D ids of the selected Cell 2D.
            for(unsigned int v = 0; v < mesh.NumberVertices[c]; c++)
            {
                vector<Vector2d> vertices_coordinates = {}; //Vector whose elemets we want to be vectors containing the coordinates of each Cell 0D.
                vertices_coordinates.reserve(mesh.NumberVertices[c]);
                vertices_coordinates.push_back(mesh.Cell0DCoordinates[v]);
                double area = PolygonArea(vertices_coordinates);
                if (area >= numeric_limits<double>::epsilon())
                {
                    cerr << "Wrong mesh" << endl;
                    return -1;
                }
            }

        }

    }
}

    bool ImportCells0D(const string& filename,PolygonalMesh& mesh)
{
    ifstream file;
    file.open(filename);

    if(file.fail())
    {
        cerr << "file not open" << endl;
        return false;
    }

    list<string> listLines;
    string line;
    while (getline(file, line))
        listLines.push_back(line);

    file.close();

    listLines.pop_front();

    mesh.NumberCell0D = listLines.size();

    if (mesh.NumberCell0D == 0)
    {
        cerr << "There is no cell 0D" << endl;
        return false;
    }

    mesh.Cell0DId.reserve(mesh.NumberCell0D);
    mesh.Cell0DCoordinates.reserve(mesh.NumberCell0D);

    for (const string& line : listLines)
    {
        istringstream converter(line);
        char delimitator = ';';
        unsigned int id;
        unsigned int marker;
        Vector2d coord;

        converter >> id >> delimitator >> marker >> delimitator >> coord(0) >> delimitator >> coord(1);

        mesh.Cell0DId.push_back(id);
        mesh.Cell0DCoordinates.push_back(coord);

        if( marker != 0)
        {
            auto ret = mesh.Cell0DMarkers.insert({marker, {id}});
            if(!ret.second)
                (ret.first)->second.push_back(id);
        }

    }
    file.close();
    return true;
}

bool ImportCell1Ds(const string &filename, PolygonalMesh& mesh)
{

    ifstream file;
    file.open(filename);

    if(file.fail())
        return false;

    list<string> listLines;
    string line;
    while (getline(file, line))
        listLines.push_back(line);

    listLines.pop_front();

    mesh.NumberCell1D = listLines.size();

    if (mesh.NumberCell1D == 0)
    {
        cerr << "There is no cell 1D" << endl;
        return false;
    }

    mesh.Cell1DId.reserve(mesh.NumberCell1D);
    mesh.Cell1DVertices.reserve(mesh.NumberCell1D);

    for (const string& line : listLines)
    {
        istringstream converter(line);

        char delimitator = ';';
        unsigned int id;
        unsigned int marker;
        Vector2i vertices;

        converter >> id >> delimitator >> marker >> delimitator >> vertices(0) >> delimitator >> vertices(1);

        mesh.Cell1DId.push_back(id);
        mesh.Cell1DVertices.push_back(vertices);

        if( marker != 0)
        {
            auto ret = mesh.Cell1DMarkers.insert({marker, {id}});
            if(!ret.second)
                (ret.first)->second.push_back(id);
        }
    }

    file.close();

    return true;
}

bool ImportCells2D (const string& filename, PolygonalMesh& mesh)
{
    ifstream file;
    file.open(filename);

    if(file.fail())
        return false;

    list<string> listLines;
    string line;
    while (getline(file, line))
        listLines.push_back(line);

    listLines.pop_front();
    file.close();

    mesh.NumberCell2D = listLines.size();
    if (mesh.NumberCell2D == 0)
    {
        cerr << "There is no cell 2D" << endl;
        return false;
    }

    mesh.Cell2DId.reserve(mesh.NumberCell2D);
    mesh.Cell2DVertices.reserve(mesh.NumberCell2D);
    mesh.Cell2DEdges.reserve(mesh.NumberCell2D);
    mesh.NumberEdges.reserve(mesh.NumberCell2D);
    mesh.NumberVertices.reserve(mesh.NumberCell2D);

    for (const string& line : listLines)
    {
        istringstream converter(line);

        char delimitator = ';';
        unsigned int id;
        unsigned int marker;
        unsigned int numberVertices;
        unsigned int numberEdges;


        if(!(converter >> id >> delimitator >> marker >> delimitator >> numberVertices))
        {
            cerr << "Error parsing line: " << line << endl;
            return false;
        }

        mesh.Cell2DId.push_back(id);
        mesh.NumberVertices.push_back(numberVertices);

        vector<unsigned int> vertices(numberVertices);
        for (unsigned int i = 0; i < numberVertices; i++)
        {
            converter >> delimitator;
            if(!(converter >> vertices[i]))
            {
                cerr << "Error parsing vertices in line: " << line << endl;
                return false;
            }
        }

        mesh.Cell2DVertices.push_back(vertices);

        if(!(converter >> delimitator >> numberEdges))
        {
            cerr << "Error parsing number of edges in line: " << line << endl;
            return false;
        }

        mesh.NumberEdges.push_back(numberEdges);
        vector<unsigned int> edges(numberEdges);
        for(unsigned int i = 0; i < numberEdges; i++)
        {
            converter >> delimitator;
            if(!(converter >> edges[i]))
            {
                cerr << "Error parsing edges in line: " << line << endl;
                return false;
            }
        }

        mesh.Cell2DEdges.push_back(edges);
    }

    return true;
}
}


