#pragma once

#include <stdint.h>
#include <string>
#include "../../lib/include/vec3.h"

class Config {
public:
	static void readConfig();
	// processes
	// 0 - no processes created, 1 and more - amount of manually created processes
	static size_t parallelProcesses;

	//path
	static std::string projectDir;

	// geometry
	static Vec3 bodySize;
	static Vec3 meshStep;
	static double powderThickness;

	//time
	static double startTime;
	static double timeStep;
	static double endTime;

	// logging
	static uint32_t desiredLogEntries;

	// temperatures
	static double airTemp;
	static double initialTemp;
	static double meltingTemp;

	// temperature flow
	static double fusionEnthalpy;
	static double solidRho;
	static double packingRho;
	static double liquidRho;
	static double solidC;
	static double liquidC;
	static double solidKA;
	static double solidKB;
	static double liquidKA;
	static double liquidKB;

	// radiant properties
	static double stefanBoltzmannConst;
	static double emmisivity;

	// laser beam
	static Vec3 laserVec;
	static Vec3 laserVel;
	static double laserRadius;
	static double laserPower;
	static bool laserState;

	// calculated
	//const Vec3 laserVelScaled = laserVel.multiply(timeStep);
	static double sigmoidConst;
	static double solidMass;
	static double liquidMass;
	static double enthalpyMinus;
	static double enthalpyPlus;
	static double mscs;
	static double mlcl;
	static double mscsRev;
	static double mlclRev;
	static double surfaceArea;
	static double radiantFluxConst;
	static double airTemp4;
};