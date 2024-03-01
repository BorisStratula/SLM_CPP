#include "../include/body_data.h"

BodyData::BodyData(const Mesh* const MESH) {
	totalNodes = MESH->nodesArraySize;
	totalElems = MESH->elemsArraySize;
	nodalID = std::vector<double>(totalNodes);
	nodalCoords = std::vector<double>(totalNodes * 3);
	elemID = std::vector<double>(totalElems);
	elemVertices = std::vector<int>(totalElems * 8);
	H = std::vector<double>(totalElems);
	k = std::vector<double>(totalElems);
	T = std::vector<double>(totalElems);
	state = std::vector<double>(totalElems);
	underLaser = std::vector<double>(totalElems);
	qDebug = std::vector<double>(totalElems);
	MDebug = std::vector<double>(totalElems);
	timesMelted = std::vector<double>(totalElems);
	//nodalID = new double[totalNodes];
	//nodalCoords = new double[totalNodes * 3];
	//elemID = new double[totalElems];
	//elemVertices = new int[totalElems * 8];
	//H = new double[totalElems];
	//k = new double[totalElems];
	//T = new double[totalElems];
	//state = new double[totalElems];
	//underLaser = new double[totalElems];
	//qDebug = new double[totalElems];
	//MDebug = new double[totalElems];
	//timesMelted = new double[totalElems];
	TMax = 0;
	extractMeshData(MESH);
	advance(MESH);
}

BodyData::~BodyData() {
	//delete[] nodalID;
	//delete[] nodalCoords;
	//delete[] elemID;
	//delete[] elemVertices;
	//delete[] H;
	//delete[] k;
	//delete[] T;
	//delete[] state;
	//delete[] underLaser;
	//delete[] qDebug;
	//delete[] MDebug;
	//delete[] timesMelted;
}

void BodyData::advance(const Mesh* const MESH) {
	uint32_t totalElems = MESH->elemsArraySize;
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
		state[elem] = MESH->elems[elem].state;
		//if (MESH->elems[elem].state == "powder") {
		//	state[elem] = 0;
		//}
		//else if (MESH->elems[elem].state == "liquid") {
		//	state[elem] = 1;
		//}
		//else {
		//	state[elem] = 2;
		//}
	}
}

void BodyData::extractMeshData(const Mesh* const MESH) {
	for (size_t node = 0; node < totalNodes; node++) {
		nodalID[node] = (double)node;
		nodalCoords[node * 3 + 0] = (double)MESH->nodes[node].vec.x;
		nodalCoords[node * 3 + 1] = (double)MESH->nodes[node].vec.y;
		nodalCoords[node * 3 + 2] = (double)MESH->nodes[node].vec.z;
	}
	for (size_t elem = 0; elem < totalElems; elem++) {
		elemID[elem] = (double)elem;
		for (uint32_t i = 0; i < 8; i++) {
			elemVertices[elem * 8 + i] = (int)MESH->elems[elem].vertices[i];
		}
	}
}