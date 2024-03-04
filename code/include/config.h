#pragma once

#include <stdint.h>
#include <string>
#include "../../lib/include/vec3.h"

namespace config {
	// processes
	// 0 - no processes created, 1 and more - amount of manually created processes
	extern size_t parallelProcesses;// = 8;

	//path
	extern std::string projectDir;

	// geometry
	extern Vec3 bodySize;
	extern Vec3 meshStep;
	extern double powderThickness;

	//time
	extern double startTime;
	extern double timeStep;
	extern double endTime;

	// logging
	extern uint32_t desiredLogEntries;

	// temperatures
	extern double airTemp;
	extern double initialTemp;
	extern double meltingTemp;

	// temperature flow
	extern double fusionEnthalpy;
	extern double solidRho;
	extern double packingRho;
	extern double liquidRho;
	extern double solidC;
	extern double liquidC;
	extern double solidKA;
	extern double solidKB;
	extern double liquidKA;
	extern double liquidKB;

	// radiant properties
	extern double stefanBoltzmannConst;
	extern double emmisivity;

	// laser beam
	extern Vec3 laserVec;
	extern Vec3 laserVel;
	extern double laserRadius;
	extern double laserPower;
	extern bool laserState;

	// calculated
	//const Vec3 laserVelScaled = laserVel.multiply(timeStep);
	extern double sigmoidConst;
	extern double solidMass;
	extern double liquidMass;
	extern double enthalpyMinus;
	extern double enthalpyPlus;
	extern double mscs;
	extern double mlcl;
	extern double mscsRev;
	extern double mlclRev;
	extern double surfaceArea;
	extern double radiantFluxConst;
	extern double airTemp4;
}