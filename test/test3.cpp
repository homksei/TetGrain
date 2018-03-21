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
    
    saveMFEM_meshFile("/Users/Dr.Iohanson/Desktop/grmesh.mesh", &gmsh); //Здесь нужно написать путь к директории, в которой будет сохраняться файл сетки
    
    // readNodeFile("/Users/Dr.Iohanson/Desktop/cube.node", &gmsh1);
    // saveFaceFile("/Users/Dr.Iohanson/Desktop/testFace.mesh", &gmsh1);
    
    
    
    
    return 0;
}

