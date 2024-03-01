#define _USE_MATH_DEFINES
#include <math.h>
#include "../include/laser.h"
#include "../include/config.h"
#include "../include/element.h"

Laser::Laser() {
	vec = config::laserVec;
	vel = config::laserVel;
	velScaled = vel.multiply(config::timeStep);
	radius = config::laserRadius;
	power = config::laserPower;
	state = config::laserState;
	precalcConst = config::surfaceArea * power / M_PI / pow(radius, 2.0);
}

Laser::~Laser() {

}

void Laser::advance() {
	vec = vec + velScaled;
}

double Laser::heatToElem(Elem* const ELEM) const {
	double distance = (vec - ELEM->vec).len();
	if (!state) {
		return 0;
	}
	else if (distance > radius) {
		ELEM->underLaser = 0;
		return 0;
	}
	else {
		ELEM->underLaser = 1;
		return precalcConst;
	}
}