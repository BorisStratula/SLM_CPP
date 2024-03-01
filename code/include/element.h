#pragma once

#include <stdint.h>
#include "../../lib/include/vec3.h"
#include "neighbours.h"

class Mesh;
class Laser;
class IntVec3;

class Elem {
public:
	uint32_t ID = 0;
	uint32_t* vertices = nullptr;
	Neighbours neighbours;
	Neighbours neighboursTruncated;
	uint32_t onSurface = 0;
	Vec3 vec;
	uint32_t state = 0; // 0 == powder, 1 == liquid, 2 == solid
	uint32_t underLaser = 0;
	uint32_t timesMelted = 0;
	double T = 0.0;
	double k = 0.0;
	double H = 0.0;
	double HFlow = 0.0;
	double qDebug = 0.0;
	double MDebug = 0.0;

	~Elem();

	bool init(uint32_t ID, const IntVec3& INDEX_VECTOR, const Neighbours& NEIGHBOURS, const Neighbours& NEIGHBOURS_TRUNCATED, const uint32_t STATE);
	bool valid() const;

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