#pragma once

#include <stdint.h>
#include <string>
#include <vector>
#include "neighbours.h"
#include "vec3.h"

class Mesh;
class Laser;
class IntVec3;

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
	Elem(uint32_t ID, const IntVec3& INDEX_VECTOR, const Neighbours& NEIGHBOURS, const Neighbours& NEIGHBOURS_TRUNCATED, const std::string& STATE);
	~Elem();

	double thermalConductivity() const;
	double TofH() const;
	double HofT() const;
	double enthalpyFlow(const Mesh* const MESH, const Laser* const LASER);
	double thetaI(int32_t forwardID, int32_t backwardID, uint32_t axis, const Mesh* const MESH) const;
	double thetaF(int32_t forwardID, const Mesh* const MESH) const;
	double thetaB(int32_t backwardID, const Mesh* const MESH) const;
	double radiantFlux() const;
	void chechState();
};