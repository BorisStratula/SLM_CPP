#pragma once

#include <vector>
#include "mesh.h"

class BodyData {
public:
	uint32_t totalNodes;
	uint32_t totalElems;
	std::vector<double> nodalID;
	std::vector<double> nodalCoords;
	std::vector<double> elemID;
	std::vector<int> elemVertices;
	std::vector<double> H;
	std::vector<double> k;
	std::vector<double> T;
	double TMax;
	std::vector<double> state;
	std::vector<double> underLaser;
	std::vector<double> qDebug;
	std::vector<double> MDebug;
	std::vector<double> timesMelted;


	BodyData(const Mesh* const MESH);
	~BodyData();

	void advance(const Mesh* const MESH);
	void extractMeshData(const Mesh* const MESH);
};