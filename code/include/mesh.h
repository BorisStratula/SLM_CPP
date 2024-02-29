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
	std::vector<Node> nodes;
	std::vector<Elem> elems;
	IntVec3 resolution;
	uint32_t powderLayers;
	uint32_t startPowderAtLayer;

	Mesh();
	~Mesh();

	void advance(const Laser* const LASER);
	void createElement(IntVec3 indexVec, Neighbours neighbours, Neighbours neighboursTruncated, std::string state);
	void createNode(uint32_t nodePos, Vec3 anchorVec);
	uint32_t findNodeForElement(uint32_t nodePos, Vec3 elemVec, Neighbours neighbours);
	void createMesh();
};