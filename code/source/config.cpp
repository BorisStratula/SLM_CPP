#include <fstream>
#include <iostream>

#include "../../lib/include/json.hpp"

#include "../include/config.h"


namespace config {
	double sigmoid(double x) {
		return 1 / (1 + std::exp(-12.0 * x + 6.0));
	}

	using json = nlohmann::json;
	std::ifstream rawFile("c:/Archive/Coding/C/test/CMakeProject1/example.json");
	json processedFile{ json::parse(rawFile) };
	//

	// processes
	// 0 - no processes created, 1 and more - amount of manually created processes
	size_t parallelProcesses{ processedFile["processes"]["in parallel"] };

	//path
	std::string projectDir{ processedFile["path to project"] };

	// geometry
	double x1{ processedFile["geometry"]["size"]["x"] };
	double y1{ processedFile["geometry"]["size"]["y"] };
	double z1{ processedFile["geometry"]["size"]["z"] };
	double x2 = { processedFile["geometry"]["step"]["x"] };
	double y2 = { processedFile["geometry"]["step"]["y"] };
	double z2 = { processedFile["geometry"]["step"]["z"] };
	double pt{ processedFile["geometry"]["powder thickness"] };
	Vec3 bodySize = Vec3(x1, y1, z1);
	Vec3 meshStep = Vec3(x2, y2, z2);
	double powderThickness = pt;

	//time
	double st{ processedFile["time"]["start"] };
	double ts{ processedFile["time"]["step"] };
	double iter{ processedFile["time"]["iterations"] };
	double startTime = st;
	double timeStep = ts;
	double endTime = timeStep * iter;

	// logging
	uint32_t dle{ processedFile["logging"]["log entries"] };
	uint32_t desiredLogEntries = dle;

	// temperatures
	double at{ processedFile["temperatures"]["air"] };
	double it{ processedFile["temperatures"]["initial"] };
	double mt{ processedFile["temperatures"]["melting"] };
	double airTemp = at;
	double initialTemp = it;
	double meltingTemp = mt;

	// temperature flow
	double fe{ processedFile["temperature flow"]["enthalpy of fusion"] };
	double sr{ processedFile["temperature flow"]["solid rho"] };
	double pr{ processedFile["temperature flow"]["packing rho"] };
	double lr{ processedFile["temperature flow"]["liquid rho"] };
	double sc{ processedFile["temperature flow"]["solid C"] };
	double lc{ processedFile["temperature flow"]["liquid C"] };
	double ska{ processedFile["temperature flow"]["solid KA"] };
	double skb{ processedFile["temperature flow"]["solid KB"] };
	double lka{ processedFile["temperature flow"]["liquid KA"] };
	double lkb{ processedFile["temperature flow"]["liquid KB"] };
	double fusionEnthalpy = fe;
	double solidRho = sr;
	double packingRho = pr;
	double liquidRho = lr;
	double solidC = sc;
	double liquidC = lc;
	double solidKA = ska;
	double solidKB = skb;
	double liquidKA = lka;
	double liquidKB = lkb;

	// radiant properties
	double emm{ processedFile["radiant properties"]["emmisivity"] };
	double stefanBoltzmannConst = 5.67e-8;
	double emmisivity = emm;

	// laser beam
	double x3{ processedFile["laser beam"]["location"]["relative x"] };
	double y3{ processedFile["laser beam"]["location"]["relative y"] };
	double z3{ processedFile["laser beam"]["location"]["relative z"] };
	double vx{ processedFile["laser beam"]["velocity"]["x"] };
	double vy{ processedFile["laser beam"]["velocity"]["y"] };
	double vz{ processedFile["laser beam"]["velocity"]["z"] };
	double r{ processedFile["laser beam"]["radius"] };
	double p{ processedFile["laser beam"]["power"] };
	bool s{ processedFile["laser beam"]["is ON"] };
	Vec3 laserVec = bodySize.dot(Vec3(x3, y3, z3));
	Vec3 laserVel = Vec3(vx, vy, vz);
	double laserRadius = r;
	double laserPower = p;
	bool laserState = s;

	// calculated
	//const Vec3 laserVelScaled = laserVel.multiply(timeStep);
	double sigmoidConst = sigmoid(packingRho);//0.858148935;
	double solidMass = meshStep.x * meshStep.y * meshStep.z * solidRho;
	double liquidMass = meshStep.x * meshStep.y * meshStep.z * liquidRho;
	double enthalpyMinus = solidMass * solidC * meltingTemp;
	double enthalpyPlus = enthalpyMinus + solidMass * fusionEnthalpy;
	double mscs = solidMass * solidC;
	double mlcl = liquidMass * liquidC;
	double mscsRev = 1 / mscs;
	double mlclRev = 1 / mlcl;
	double surfaceArea = meshStep.x * meshStep.y;
	double radiantFluxConst = stefanBoltzmannConst * emmisivity;
	double airTemp4 = airTemp * airTemp * airTemp * airTemp;
}