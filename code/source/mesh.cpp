#include <iostream>
#include "../../lib/include/timer.h"
#include "../include/mesh.h"
#include "../include/config.h"
#include "../include/laser.h"
#include "../include/body_data.h"
#include "../include/Processor.h"

Mesh::Mesh(Laser* const LASER) {
	resolution = returnResolution();
	powderLayers = (uint32_t)round(Config::powderThickness / Config::meshStep.z);
	startPowderAtLayer = (uint32_t)resolution.z - powderLayers;
	nodesArraySize = (1 + resolution.x) * (1 + resolution.y) * (1 + resolution.z);
	elemsArraySize = resolution.x * resolution.y * resolution.z;
	nodes = new Node[nodesArraySize](); // allocates memory for array of nodes and calls the default constructor
	elems = new Elem[elemsArraySize]; // allocates memory and calls nothing
	memset(elems, 0, sizeof(Elem) * elemsArraySize); // filling the allocated memory with zeroes by hands
	currentNodeID = 0;
	currentElemID = 0;
	Elem::meshPtr = this;
	Elem::laserPtr = LASER;
	createMesh();
	if (Config::parallelProcesses != 0) {
		processors = new Processor * [Config::parallelProcesses];
		for (size_t i = 0; i < Config::parallelProcesses; i++) {
			processors[i] = new Processor();
		}
		splitter.dataSize = elemsArraySize;
		if (splitter.dataSize % Config::parallelProcesses == 0) splitter.chunkSize = splitter.dataSize / Config::parallelProcesses;
		else splitter.chunkSize = splitter.dataSize / Config::parallelProcesses / 2 + 1;
		splitter.dataStep = splitter.chunkSize * Config::parallelProcesses;
		splitter.overflow = splitter.dataSize - 2 * splitter.dataStep;
	}
	else {
		processors = nullptr;
	}
}

Mesh::~Mesh() {
	if (nodes) delete[] nodes;
	if (elems) delete[] elems;
	for (size_t i = 0; i < Config::parallelProcesses; i++) {
		if (processors[i]) delete processors[i];
	}
	if (processors) delete processors;
}

void Mesh::advance() {
	if (Config::parallelProcesses == 0) advanceClassic();
	else advanceInParallel();
}

void Mesh::advanceClassic() {
	for (uint32_t elem = 0; elem < elemsArraySize; elem++) {
		elems[elem].calcStep1();
	}
	for (uint32_t elem = 0; elem < elemsArraySize; elem++) {
		elems[elem].calcStep2();
	}
}

void Mesh::advanceInParallel() {
	splitter.processedData = 0;
	while (1) {
		for (size_t i = 0; i < Config::parallelProcesses; i++) {
			processors[i]->putData(&elems[i * splitter.chunkSize + splitter.processedData], splitter.chunkSize);
		}
		splitter.processedData += splitter.dataStep;
		while (1) {
			bool finished = true;
			for (size_t i = 0; i < Config::parallelProcesses; i++) {
				if (!processors[i]->isReady()) {
					finished = false;
					break;
				}
			}
			if (finished) {
				break;
			}
			else {
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
			}
		}
		if (splitter.processedData == splitter.dataSize) {
			break;
		}
		splitter.processedData += splitter.overflow; // overlap correction when we have to do 2 passes because we can not divide data between processes for only 1 pass
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
	int32_t xRes = (uint32_t)round(Config::bodySize.x / Config::meshStep.x);
	int32_t yRes = (uint32_t)round(Config::bodySize.y / Config::meshStep.y);
	int32_t zRes = (uint32_t)round(Config::bodySize.z / Config::meshStep.z);
	return IntVec3(xRes, yRes, zRes);
}