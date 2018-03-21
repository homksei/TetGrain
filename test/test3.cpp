#include "common.h"
#include "grainmesh.h"
#include "iotetgen.h"
#include <iostream>


using namespace grain;
int main(int argc, char** argv)
{
    GrainMesh gmsh;
    GrainMesh gmsh1;
    
    // Geerate stub tetrahedron
    gmsh.makeStubTetra();
    
    saveMFEM_meshFile("/Users/Dr.Iohanson/Desktop/grmesh.mesh", &gmsh);
    // readNodeFile("/Users/Dr.Iohanson/Desktop/cube.node", &gmsh1);
    // saveFaceFile("/Users/Dr.Iohanson/Desktop/testFace.mesh", &gmsh1);
    int a = 5;
    
    
    
    
    return 0;
}

