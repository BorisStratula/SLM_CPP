#include <iostream>
#include "../../lib/include/timer.h"
#include "../include/mesh.h"
#include "../include/config.h"
#include "../include/laser.h"
#include "../include/body_data.h"

Mesh::Mesh() {
	resolution = returnResolution();
	powderLayers = (uint32_t)round(config::powderThickness / config::meshStep.z);
	startPowderAtLayer = (uint32_t)resolution.z - powderLayers;
	nodesArraySize = (1 + resolution.x) * (1 + resolution.y) * (1 + resolution.z);
	elemsArraySize = resolution.x * resolution.y * resolution.z;
	nodes = new Node[nodesArraySize](); // allocates memory for array of nodes and calls the default constructor
	elems = new Elem[elemsArraySize]; // allocates memory and calls nothing
	memset(elems, 0, sizeof(Elem) * elemsArraySize); // filling the allocated memory with zeroes by hands
	currentNodeID = 0;
	currentElemID = 0;
	createMesh();
}

Mesh::~Mesh() {
	if (nodes) delete[] nodes;
	if (elems) delete[] elems;
}

void Mesh::advance(const Laser* const LASER, BodyData* const bodyData) {
	for (uint32_t elem = 0; elem < elemsArraySize; elem++) {
		elems[elem].HFlow = elems[elem].enthalpyFlow(this, LASER);
	}
	for (uint32_t elem = 0; elem < elemsArraySize; elem++) {
		elems[elem].H += elems[elem].HFlow;
		elems[elem].T = elems[elem].TofH();
		elems[elem].k = elems[elem].thermalConductivity();
		elems[elem].chechState();
		if (elems[elem].T > bodyData->TMax) bodyData->TMax = elems[elem].T;
	}
}

void Mesh::createElement(uint32_t elemID, const IntVec3& INDEX_VEC, const Neighbours& NEIGHBOURS, const Neighbours& NEIGHBOURS_TRUNCATED, const uint32_t STATE) {
	uint32_t nodeID;
	auto* newElem = &elems[elemID];
	if (newElem->init(elemID, INDEX_VEC, NEIGHBOURS, NEIGHBOURS_TRUNCATED, STATE) != true) {
		std::exit(2);
	}
	else {
		for (uint32_t nodePos = 0; nodePos < 8; nodePos++) {
			nodeID = findNodeForElement(nodePos, newElem->vec, NEIGHBOURS);
			newElem->vertices[nodePos] = nodeID;
		}
	}
}

void Mesh::createNode(uint32_t nodeID, uint32_t nodePos, const Vec3& ANCHOR_VEC) {
	Vec3 vec = Node::nodalVec(nodePos, ANCHOR_VEC);
	nodes[nodeID] = Node(nodeID, vec);
}

uint32_t Mesh::findNodeForElement(uint32_t nodePos, const Vec3& ELEM_VEC, const Neighbours& NEIGHBOURS) {
	const int32_t RELATIONS[8][3] = { {3, 1, 4},{2, -1, 5},{-1, -1, 6},{-1, 2, 7},{7, 5, -1},{6, -1, -1},{-1, -1, -1},{-1, 6, -1} };
	uint32_t nodeID;
	if (NEIGHBOURS.xMinus != -1 and RELATIONS[nodePos][0] != -1) {
		nodeID = elems[NEIGHBOURS.xMinus].vertices[RELATIONS[nodePos][0]];
	}
	else if (NEIGHBOURS.yMinus != -1 and RELATIONS[nodePos][1] != -1) {
		nodeID = elems[NEIGHBOURS.yMinus].vertices[RELATIONS[nodePos][1]];
	}
	else if (NEIGHBOURS.zMinus != -1 and RELATIONS[nodePos][2] != -1) {
		nodeID = elems[NEIGHBOURS.zMinus].vertices[RELATIONS[nodePos][2]];
	}
	else {
		nodeID = currentNodeID++;
		createNode(nodeID, nodePos, ELEM_VEC);
	}
	return nodeID;
}

void Mesh::createMesh() {
	Timer timer = Timer();
	timer.start();
	uint32_t elemID = 0;
	IntVec3 indexVector = IntVec3();
	Neighbours neighbours = Neighbours();
	Neighbours neighboursTruncated = Neighbours();
	uint32_t state = 2;
	for (indexVector.z = 0; indexVector.z < resolution.z; indexVector.z++) {
		if (indexVector.z >= (int32_t)startPowderAtLayer) {
			state = 0;
		}
		for (indexVector.y = 0; indexVector.y < resolution.y; indexVector.y++) {
			for (indexVector.x = 0; indexVector.x < resolution.x; indexVector.x++) {
				neighbours = Neighbours(indexVector, elemID, resolution);
				neighboursTruncated = neighbours;
				neighboursTruncated.truncate();
				createElement(elemID, indexVector, neighbours, neighboursTruncated, state);
				elemID += 1;
			}
		}
	}
	std::cout << "mesh created, " << timer.formatElapsed() << " elems = " << elemsArraySize << " nodes = " << nodesArraySize << '\n';
}

IntVec3 Mesh::returnResolution() const {
	int32_t xRes = (uint32_t)round(config::bodySize.x / config::meshStep.x);
	int32_t yRes = (uint32_t)round(config::bodySize.y / config::meshStep.y);
	int32_t zRes = (uint32_t)round(config::bodySize.z / config::meshStep.z);
	return IntVec3(xRes, yRes, zRes);
}