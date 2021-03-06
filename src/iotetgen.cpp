#include "iotetgen.h"
#include <fstream>

int grain::readNodeFile(std::string filename, GrainMesh * mesh)
{
	std::ifstream fin(filename, std::ifstream::in);
	int pCount = 0;
	int trash = 0;
	fin >> pCount;
	fin >> trash;
	fin >> trash;
	fin >> trash;
	fin >> trash;
	std::vector<vec3d> newPoints(pCount);
	std::vector<char> newPLabels(pCount);
	for (int i = 0; i < pCount; i++)
	{
		fin >> trash
			>> newPoints[i].x
			>> newPoints[i].y
			>> newPoints[i].z
			>> newPLabels[i];
	}
	fin.close();
	mesh->setVertices(newPoints);
	mesh->setVerticesLabels(newPLabels);
	return 0;
}

int grain::readFaceFile(std::string filename, GrainMesh *mesh)
{
	std::ifstream fin(filename, std::ifstream::in);
	int fCount = 0;
	int trash = 0;
	bool marker = false;

	fin >> fCount;
	fin >> trash;

	if (trash == 1) // Have boundary marker
		marker = true;
	std::vector<vec3i> newFacet(fCount);
	std::vector<char> newFLabels(fCount);

	for (int i = 0; i < fCount; i++)
	{
		fin >> trash
			>> newFacet[i].x
			>> newFacet[i].y
			>> newFacet[i].z;
		if (marker)
			fin >> newFLabels[i];

		// Vertices in file start from 1, not from 0
		newFacet[i].x--;
		newFacet[i].y--;
		newFacet[i].z--;
	}
	fin.close();
	mesh->setTriangles(newFacet);
	if (marker)
		mesh->setTrianglesLabels(newFLabels);

	return 0;
}

int grain::readEleFile(std::string filename, GrainMesh *mesh)
{
	std::ifstream fin(filename, std::ifstream::in);
	int tCount = 0;
	int trash = 0;
	fin >> tCount;
	fin >> trash;
	fin >> trash;
	std::vector<vec4i> newTetra(tCount);
	std::vector<char> newTLabels(tCount);
	for (int i = 0; i < tCount; i++)
	{
		fin >> trash
			>> newTetra[i].x
			>> newTetra[i].y
			>> newTetra[i].z
			>> newTetra[i].w
			>> newTLabels[i];
	}
	fin.close();
	mesh->setTetra(newTetra);
	mesh->setTetraLabels(newTLabels);
	return 0;
}

int grain::saveNodeFile(std::string filename, GrainMesh *mesh)
{
	int mPointsCount = mesh->getVerticesCount();
	std::vector<vec3d>* mPoints = mesh->getVertices();
	std::vector<char>* mPointsLabels = mesh->getVerticesLabels();
	bool marker = false;
	if (mPointsLabels->size() == mPoints->size())
		marker = true;

	std::ofstream fout(filename);
	fout << mPointsCount << " " << 3 << " " << 0 << " " << 0 << " " << 0 << "\n";
	for (int i = 0; i < mPointsCount; i++)
	{
		fout << i + 1 << " "
			<< mPoints->at(i).x << " "
			<< mPoints->at(i).y << " "
			<< mPoints->at(i).z;
		if (marker)
			fout << " " << (int)mPointsLabels->at(i) << "\n";
		else
			fout << "\n";
	}
	fout.close();
	return 0;
}

int grain::saveFaceFile(std::string filename, GrainMesh * mesh)
{
	int mTrianglesCount = mesh->getTrianglesCount();
	std::vector<vec3i>* mTriangles = mesh->getTriangles();
	std::vector<char>* mTrianglesLabels = mesh->getTrianglesLabels();
	bool marker = false;
	if (mTrianglesLabels->size() == mTriangles->size())
		marker = true;

	std::ofstream fout(filename);
	fout << mTrianglesCount;
	if (marker)
		fout << " 1\n";
	else
		fout << " 0\n";
	for (int i = 0; i < mTrianglesCount; i++)
	{
		fout << i << " "
			<< mTriangles->at(i).x << " "
			<< mTriangles->at(i).y << " "
			<< mTriangles->at(i).z;
		if (marker)
			fout << " " << (int)mTrianglesLabels->at(i) << "\n";
		else
			fout << "\n";
	}
	fout.close();

	return 0;
}

int grain::saveEleFile(std::string filename, GrainMesh *mesh)
{
	int mTetraCount = mesh->getTetraCount();
	std::vector<vec4i>* mTetra = mesh->getTetra();
	std::vector<char>* mTetraLabels = mesh->getTetraLabels();
	bool marker = false;
	if (mTetraLabels->size() == mTetra->size())
		marker = true;

	std::ofstream fout(filename);
	fout << mTetraCount << " " << 4;
	if (marker)
		fout << " 1\n";
	else
		fout << " 0\n";
	fout.flush();
	for (int i = 0; i < mTetraCount; i++)
	{
		fout << i << " "
			<< mTetra->at(i).x << " "
			<< mTetra->at(i).y << " "
			<< mTetra->at(i).z << " "
			<< mTetra->at(i).w;
		if (marker)
			fout << " " << (int)mTetraLabels->at(i) << "\n";
		else
			fout << "\n";
	}
	fout.close();
	return 0;
}
int grain::saveMFEM_meshFile(std::string filename, GrainMesh *mesh)
{
    std::ofstream fout(filename);
    fout<<"MFEM mesh v1.0"<<std::endl; //Инормация для MFEM, из которой он узнает, какого формата файла меша
    fout<<""<<std::endl;
    fout<<"#"<<std::endl;                                          // Строчки, которые начинаются с # обрабатываются
    fout<<"# MFEM Geometry Types (see mesh/geom.hpp):"<<std::endl; // библиотекой как комментарии.
    fout<<"#"<<std::endl;                                          // В начале описания файла меша, у сеток формата .mesh
    fout<<"# POINT       = 0"<<std::endl;                          // всегда есть информация которая представлена на строчках,
    fout<<"# SEGMENT     = 1"<<std::endl;                          // параллельных этим комментариям. В них говорится, что
    fout<<"# TRIANGLE    = 2"<<std::endl;                          // больше информации можно найти в файле geom.hpp
    fout<<"# SQUARE      = 3"<<std::endl;                          // непосредственно сам файл находится в папке с библиотекой.
    fout<<"# TETRAHEDRON = 4"<<std::endl;                          // <--На 4 строчках выше и на 1 ниже представлена справка о
    fout<<"# CUBE        = 5"<<std::endl;                          // типах геометрии
    fout<<"#"<<std::endl;
    fout<<""<<std::endl;
    fout<<"dimension"<<std::endl; //Размерность
    fout<<"2"<<std::endl;
    fout<<""<<std::endl;
    fout<<"elements"<<std::endl;
    
    
     int mTetraCount = mesh->getTetraCount();
     std::vector<vec4i>* mTetra = mesh->getTetra();
     std::vector<char>* mTetraLabels = mesh->getTetraLabels();
     bool marker2 = false;
     if (mTetraLabels->size() == mTetra->size())
     marker2 = true;
     
     fout << mTetraCount << std::endl;
     for (int i = 0; i < mTetraCount; i++)
     {
     fout << i+1 << " " <<"2";
     if (marker2)
     fout << " " << (int)mTetraLabels->at(i) << " ";
     fout<< mTetra->at(i).x << " "
     << mTetra->at(i).y << " "
     << mTetra->at(i).z << " "
     << mTetra->at(i).w<<std::endl;
     }
    
   // fout<<"1"<<std::endl;
  //  fout<<"1 2 1 3 0"<<std::endl;
    fout<<""<<std::endl;
    fout <<"boundary"<<std::endl;
    
    
    int mTrianglesCount = mesh->getTrianglesCount();
    std::vector<vec3i>* mTriangles = mesh->getTriangles();
    std::vector<char>* mTrianglesLabels = mesh->getTrianglesLabels();
    bool marker1 = false;
    if (mTrianglesLabels->size() == mTriangles->size())
        marker1 = true;
    
    fout << mTrianglesCount;
    fout <<std::endl;
    
    for (int i = 0; i < mTrianglesCount; i++)
    {
        fout << i+1 << " " << "2" << " "
        << mTriangles->at(i).x << " "
        << mTriangles->at(i).y << " "
        << mTriangles->at(i).z;
        if (marker1)
            fout << " " << (int)mTrianglesLabels->at(i) << "\n";
        else
            fout << "\n";
    }
    fout <<std::endl;
    int mPointsCount = mesh->getVerticesCount();
    std::vector<vec3d>* mPoints = mesh->getVertices();
    std::vector<char>* mPointsLabels = mesh->getVerticesLabels();
    bool marker = false;
    if (mPointsLabels->size() == mPoints->size())
        marker = true;
    
    fout <<"vertices"<<std::endl;
    fout << mPointsCount<<std::endl;
    fout <<"3"<<std::endl;
    
    for (int i = 0; i < mPointsCount; i++)
    {
        fout<< mPoints->at(i).x << " "
        << mPoints->at(i).y << " "
        << mPoints->at(i).z;
        if (marker)
            fout << " " << (int)mPointsLabels->at(i) << "\n";
        else
            fout << "\n";
    }
    fout.close();
    return 0;
}

