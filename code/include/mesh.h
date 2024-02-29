#pragma once

#include <vector>
#include <stdint.h>
#include "vec3.h"
#include "intvec3.h"
#include "node.h"
#include "element.h"

class BodyData;
class Laser;
class Simulation;

class Mesh {
public:
	IntVec3 resolution;
	uint32_t powderLayers;
	uint32_t startPowderAtLayer;
	uint32_t nodesArraySize;
	uint32_t elemsArraySize;
	Node* nodes;
	Elem* elems;
	uint32_t currentNodeID;
	uint32_t currentElemID;

	Mesh();
	~Mesh();

	void advance(const Laser* const LASER, BodyData* const bodyData);
	void createElement(uint32_t elemID, const IntVec3& INDEX_VEC, const Neighbours& NEIGHBOURS, const Neighbours& NEIGHBOURS_TRUNCATED, const std::string& STATE);
	void createNode(uint32_t nodeID, uint32_t nodePos, const Vec3& ANCHOR_VEC);
	uint32_t findNodeForElement(uint32_t nodePos, const Vec3& elemVec, const Neighbours& NEIGHBOURS);
	void createMesh();
	IntVec3 returnResolution() const;
};