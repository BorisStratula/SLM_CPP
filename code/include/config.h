#pragma once

#include <stdint.h>
#include <string>
#include "../../lib/include/vec3.h"

namespace config {
	// processes
	const size_t parallelProcesses = 1;

	//path
	const std::string projectDir = "C:/del/03";

	// geometry
	const Vec3 bodySize = Vec3(6000e-6, 20e-6, 600e-6);
	const Vec3 meshStep = Vec3(20e-6, 20e-6, 20e-6);
	const double powderThickness = 100e-6;

	//time
	const double startTime = 0.0;
	const double timeStep = 1e-10;
	const double endTime = timeStep * 1000000.0;

	// logging
	const uint32_t desiredLogEntries = 100;

	// temperatures
	const double airTemp = 293.0;
	const double initialTemp = 1500.0;
	const double meltingTemp = 1923.0;

	// temperature flow
	const double fusionEnthalpy = 286000.0;
	const double solidRho = 4400.0;
	const double packingRho = 0.65;
	const double liquidRho = 3900.0;
	const double solidC = 770.0;
	const double liquidC = 1150.0;
	const double solidKA = 0.01463;
	const double solidKB = -0.53614;
	const double liquidKA = 0.01808;
	const double liquidKB = -6.12967;

	// radiant properties
	const double stefanBoltzmannConst = 5.67e-8;
	const double emmisivity = 1.0;

	// laser beam
	const Vec3 laserVec = bodySize.dot(Vec3(0.99, 0.5, 1.0));
	const Vec3 laserVel = Vec3(-80.0, 0.0, 0.0);
	const double laserRadius = 40e-6;
	const double laserPower = 6000.0;
	const bool laserState = true;

	// calculated
	//const Vec3 laserVelScaled = laserVel.multiply(timeStep);
	const double sigmoidConst = 0.858148935;
	const double solidMass = meshStep.x * meshStep.y * meshStep.z * solidRho;
	const double liquidMass = meshStep.x * meshStep.y * meshStep.z * liquidRho;
	const double enthalpyMinus = solidMass * solidC * meltingTemp;
	const double enthalpyPlus = enthalpyMinus + solidMass * fusionEnthalpy;
	const double mscs = solidMass * solidC;
	const double mlcl = liquidMass * liquidC;
	const double mscsRev = 1 / mscs;
	const double mlclRev = 1 / mlcl;
	const double surfaceArea = meshStep.x * meshStep.y;
	const double radiantFluxConst = stefanBoltzmannConst * emmisivity;
	const double airTemp4 = airTemp * airTemp * airTemp * airTemp;
}