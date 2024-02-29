#pragma once

#include <vector>
#include <stdint.h>
#include "vec3.h"
#include "intvec3.h"
#include "node.h"
#include "element.h"

class Simulation;

class Laser;

class Mesh {
public:
	//const uint32_t N_ARRAY_SIZE;
	IntVec3 resolution;
	uint32_t powderLayers;
	uint32_t startPowderAtLayer;
	std::vector<Node> nodes;
	std::vector<Elem> elems;

	Mesh();
	~Mesh();

	void advance(const Laser* const LASER);
	void createElement(const IntVec3& INDEX_VEC, const Neighbours& NEIGHBOURS, const Neighbours& NEIGHBOURS_TRUNCATED, const std::string& STATE);
	void createNode(uint32_t nodePos, const Vec3& ANCHOR_VEC);
	uint32_t findNodeForElement(uint32_t nodePos, Vec3 elemVec, Neighbours neighbours);
	void createMesh();
};