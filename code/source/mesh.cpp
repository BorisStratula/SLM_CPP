#include <iostream>
#include "../include/mesh.h"
#include "../include/config.h"
#include "../include/timer.h"
#include "../include/simulation.h"
#include "../include/laser.h"

Mesh::Mesh() {
	int32_t xRes = (uint32_t)round(config::bodySize.x / config::meshStep.x);
	int32_t yRes = (uint32_t)round(config::bodySize.y / config::meshStep.y);
	int32_t zRes = (uint32_t)round(config::bodySize.z / config::meshStep.z);
	resolution = IntVec3(xRes, yRes, zRes);
	powderLayers = (uint32_t)round(config::powderThickness / config::meshStep.z);
	startPowderAtLayer = (uint32_t)resolution.z - powderLayers;
	nodes = std::vector<Node>();
	elems = std::vector<Elem>();
	createMesh();
}

Mesh::~Mesh() {

}

void Mesh::advance(const Laser* const LASER) {
	for (uint32_t elem = 0; elem < (uint32_t)elems.size(); elem++) {
		elems[elem].HFlow = elems[elem].enthalpyFlow(this, LASER);
	}
	for (uint32_t elem = 0; elem < (uint32_t)elems.size(); elem++) {
		elems[elem].H += elems[elem].HFlow;
		elems[elem].T = elems[elem].TofH();
		elems[elem].k = elems[elem].thermalConductivity();
		elems[elem].chechState();
	}
}

void Mesh::createElement(const IntVec3& INDEX_VEC, const Neighbours& NEIGHBOURS, const Neighbours& NEIGHBOURS_TRUNCATED, const std::string& STATE) {
	uint32_t ID = (uint32_t)elems.size();
	uint32_t nodeID;
	Elem newElem = Elem(ID, INDEX_VEC, NEIGHBOURS, NEIGHBOURS_TRUNCATED, STATE);
	for (uint32_t nodePos = 0; nodePos < 8; nodePos++) {
		nodeID = findNodeForElement(nodePos, newElem.vec, NEIGHBOURS);
		newElem.vertices[nodePos] = nodeID;
	}
	elems.push_back(newElem);
}

void Mesh::createNode(uint32_t nodePos, const Vec3& ANCHOR_VEC) {
	Vec3 vec = Node::nodalVec(nodePos, ANCHOR_VEC);
	uint32_t ID = (uint32_t)nodes.size();
	nodes.push_back(Node(ID, vec));
}

uint32_t Mesh::findNodeForElement(uint32_t nodePos, Vec3 elemVec, Neighbours neighbours) {
	std::vector<std::vector<int32_t>> relations{ {3, 1, 4},{2, -1, 5},{-1, -1, 6},{-1, 2, 7},{7, 5, -1},{6, -1, -1},{-1, -1, -1},{-1, 6, -1} };
	uint32_t nodeID;
	if (neighbours.xMinus != -1 and relations[nodePos][0] != -1) {
		nodeID = elems[neighbours.xMinus].vertices[relations[nodePos][0]];
	}
	else if (neighbours.yMinus != -1 and relations[nodePos][1] != -1) {
		nodeID = elems[neighbours.yMinus].vertices[relations[nodePos][1]];
	}
	else if (neighbours.zMinus != -1 and relations[nodePos][2] != -1) {
		nodeID = elems[neighbours.zMinus].vertices[relations[nodePos][2]];
	}
	else {
		nodeID = (uint32_t)nodes.size();
		createNode(nodePos, elemVec);
	}
	return nodeID;
}

void Mesh::createMesh() {
	Timer timer = Timer();
	timer.start();
	uint32_t ID = (uint32_t)elems.size();
	IntVec3 indexVector = IntVec3();
	Neighbours neighbours = Neighbours();
	Neighbours neighboursTruncated = Neighbours();
	std::string state = "solid";
	for (indexVector.z = 0; indexVector.z < resolution.z; indexVector.z++) {
		if (indexVector.z >= (int32_t)startPowderAtLayer) {
			state = "powder";
		}
		for (indexVector.y = 0; indexVector.y < resolution.y; indexVector.y++) {
			for (indexVector.x = 0; indexVector.x < resolution.x; indexVector.x++) {
				neighbours = Neighbours(indexVector, ID, resolution);
				neighboursTruncated = neighbours;
				neighboursTruncated.truncate();
				createElement(indexVector, neighbours, neighboursTruncated, state);
				ID += 1;
			}
		}
	}
	std::cout << "createMesh " << timer.formatElapsed() << '\n';
}