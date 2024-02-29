#pragma once

#include <stdint.h>

class IntVec3 {
public:
	int32_t x;
	int32_t y;
	int32_t z;

	IntVec3();
	IntVec3(int32_t x, int32_t y, int32_t z);
	~IntVec3();
};