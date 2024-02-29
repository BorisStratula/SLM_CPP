#pragma once

#include <string>
#include "vec3.h"
#include "element.h"

class Laser {
public:
	Vec3 vec;
	Vec3 vel;
	Vec3 velScaled;
	double radius;
	double power;
	double precalcConst;
	std::string state;

	Laser();
	~Laser();

	void advance();
	double heatToElem(Elem* const ELEM) const;
};