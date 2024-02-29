#pragma once

#include <stdint.h>
#include "vec3.h"
#include "intvec3.h"

class Neighbours {
public:
	int32_t xMinus;
	int32_t xPlus;
	int32_t yMinus;
	int32_t yPlus;
	int32_t zMinus;
	int32_t zPlus;
	int32_t origin;
	uint32_t onSurface;

	Neighbours() = default;
	Neighbours(IntVec3 indexVec, uint32_t ID, IntVec3 resolution);
	~Neighbours();

	int32_t xMinusID(IntVec3 indexVec, uint32_t ID);
	int32_t xPlusID(IntVec3 indexVec, uint32_t ID, IntVec3 resolution);
	int32_t yMinusID(IntVec3 indexVec, uint32_t ID, IntVec3 resolution);
	int32_t yPlusID(IntVec3 indexVec, uint32_t ID, IntVec3 resolution);
	int32_t zMinusID(IntVec3 indexVec, uint32_t ID, IntVec3 resolution);
	int32_t zPlusID(IntVec3 indexVec, uint32_t ID, IntVec3 resolution);
	void truncate();
};