#include "../include/body_data.h"

BodyData::BodyData(const Mesh* const MESH) {
	totalNodes = (uint32_t)MESH->nodes.size();
	totalElems = (uint32_t)MESH->elems.size();
	nodalID = std::vector<double>(totalNodes);
	nodalCoords = std::vector<double>(totalNodes * 3);
	elemID = std::vector<double>(totalElems);
	elemVertices = std::vector<int>(totalElems * 8);
	H = std::vector<double>(totalElems);
	k = std::vector<double>(totalElems);
	T = std::vector<double>(totalElems);
	TMax = 0;
	state = std::vector<double>(totalElems);
	underLaser = std::vector<double>(totalElems);
	qDebug = std::vector<double>(totalElems);
	MDebug = std::vector<double>(totalElems);
	timesMelted = std::vector<double>(totalElems);
	extractMeshData(MESH);
	advance(MESH);
}

BodyData::~BodyData() {

}

void BodyData::advance(const Mesh* const MESH) {
	uint32_t totalElems = (uint32_t)MESH->elems.size();
	TMax = 0;
	for (uint32_t elem = 0; elem < totalElems; elem++) {
		H[elem] = MESH->elems[elem].H;
		k[elem] = MESH->elems[elem].k;
		T[elem] = MESH->elems[elem].T;
		if (T[elem] > TMax) {
			TMax = T[elem];
		}
		underLaser[elem] = MESH->elems[elem].underLaser;
		qDebug[elem] = MESH->elems[elem].qDebug;
		MDebug[elem] = MESH->elems[elem].MDebug;
		timesMelted[elem] = MESH->elems[elem].timesMelted;
		if (MESH->elems[elem].state == "powder") {
			state[elem] = 0;
		}
		else if (MESH->elems[elem].state == "liquid") {
			state[elem] = 1;
		}
		else {
			state[elem] = 2;
		}
	}
}

void BodyData::extractMeshData(const Mesh* const MESH) {
	for (uint32_t node = 0; node < totalNodes; node++) {
		nodalID[node] = (double)node;
		nodalCoords[node * 3 + 0] = (double)MESH->nodes[node].vec.x;
		nodalCoords[node * 3 + 1] = (double)MESH->nodes[node].vec.y;
		nodalCoords[node * 3 + 2] = (double)MESH->nodes[node].vec.z;
	}
	for (uint32_t elem = 0; elem < totalElems; elem++) {
		elemID[elem] = (double)elem;
		for (uint32_t i = 0; i < 8; i++) {
			elemVertices[elem * 8 + i] = (int)MESH->elems[elem].vertices[i];
		}
	}
}