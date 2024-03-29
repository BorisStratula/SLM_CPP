#pragma once

#include <stdint.h>
#include "../../lib/include/vec3.h"
#include "../../lib/include/intvec3.h"
#include "node.h"
#include "element.h"

class BodyData;
class Laser;
class Simulation;
class Processor;

class Mesh {
public:

	struct Splitter {
		uint32_t dataSize;
		uint32_t chunkSize;
		uint32_t processedData;
		uint32_t dataStep;
		int32_t overflow;
	};

	Splitter splitter;

	IntVec3 resolution;
	uint32_t powderLayers;
	uint32_t startPowderAtLayer;
	uint32_t nodesArraySize;
	uint32_t elemsArraySize;
	Node* nodes = nullptr;
	Elem* elems = nullptr;
	uint32_t currentNodeID;
	uint32_t currentElemID;
	Processor** processors = nullptr;

	Mesh(Laser* const LASER);
	~Mesh();

	void advance();
	void advanceClassic();
	void advanceInParallel();
	void createElement(uint32_t elemID, const IntVec3& INDEX_VEC, const Neighbours& NEIGHBOURS, const Neighbours& NEIGHBOURS_TRUNCATED, const uint32_t STATE);
	void createNode(uint32_t nodeID, uint32_t nodePos, const Vec3& ANCHOR_VEC);
	uint32_t findNodeForElement(uint32_t nodePos, const Vec3& elemVec, const Neighbours& NEIGHBOURS);
	void createMesh();
	IntVec3 returnResolution() const;
};