#pragma once

#include <string>
#include "vec3.h"

class Elem;

class Laser {
public:
	Vec3 vec;
	Vec3 vel;
	Vec3 velScaled;
	double radius;
	double power;
	double precalcConst;
	bool state;

	Laser();
	~Laser();

	void advance();
	double heatToElem(Elem* const ELEM) const;
};