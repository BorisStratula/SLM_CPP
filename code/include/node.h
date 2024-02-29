#pragma once

#include <stdint.h>
#include "vec3.h"

class Node {
public:
	uint32_t ID;
	Vec3 vec;

	Node();
	Node(uint32_t ID, const Vec3& VEC);
	~Node();
	
	static Vec3 nodalVec(uint32_t cornerNumber, const Vec3& ANCHOR_VEC);
};
