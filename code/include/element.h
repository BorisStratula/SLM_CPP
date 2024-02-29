#pragma once

#include <stdint.h>
#include <string>
#include <vector>
#include "neighbours.h"
#include "vec3.h"
#include "intvec3.h"

class Mesh;

class Laser;

class Elem {
public:
	uint32_t ID;
	std::vector<uint32_t> vertices;
	Neighbours neighbours;
	Neighbours neighboursTruncated;
	uint32_t onSurface = neighboursTruncated.onSurface;
	Vec3 vec;
	double T;
	std::string state;
	uint32_t underLaser;
	uint32_t timesMelted;
	double k;
	double H;
	double HFlow;
	double qDebug;
	double MDebug;

	Elem() = default;
	Elem(uint32_t ID, IntVec3 indexVector, Neighbours neighbours, Neighbours neighboursTruncated, std::string state);
	~Elem();

	double thermalConductivity();
	double TofH();
	double HofT();
	double enthalpyFlow(const Mesh* const MESH, const Laser* const LASER);
	double thetaI(int32_t forwardID, int32_t backwardID, uint32_t axis, const Mesh* const MESH);
	double thetaF(int32_t forwardID, const Mesh* const MESH);
	double thetaB(int32_t backwardID, const Mesh* const MESH);
	double radiantFlux();
	void chechState();
};