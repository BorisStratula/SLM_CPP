#include <fstream>
#include <iostream>

#include "../../lib/include/json.hpp"

#include "../include/config.h"

void Config::readConfig() {
	using json = nlohmann::json;
	auto currentPath = std::filesystem::current_path();
	std::string exePath = currentPath.generic_string();
	std::string localPath = exePath + "/config.json";
	std::string externalPath = "c:/Archive/Work/ICAD/2024/modeling/slm_cpp/config.json";
	std::ifstream localFile(localPath);
	std::ifstream externalFile(externalPath);
	std::string* stringPtr = nullptr;
	if (localFile) {
		stringPtr = &localPath;
		printf("Local config file found\n");
	}
	else if (externalFile) {
		stringPtr = &externalPath;
		printf("No local config, but external config file found\n");
	}
	else {
		printf("No config file\n");
		delete stringPtr;
		exit(3);
	}
	std::ifstream configFile(*stringPtr);
	json processedFile{ json::parse(configFile) };
	//

	// processes
	// 0 - no processes created, 1 and more - amount of manually created processes
	size_t pp{ processedFile["processes"]["in parallel"] };
	parallelProcesses = pp;

	//path
	std::string prDir{ processedFile["path to project"] };
	projectDir = prDir;

	// geometry
	double x1{ processedFile["geometry"]["size"]["x"] };
	double y1{ processedFile["geometry"]["size"]["y"] };
	double z1{ processedFile["geometry"]["size"]["z"] };
	double x2 = { processedFile["geometry"]["step"]["x"] };
	double y2 = { processedFile["geometry"]["step"]["y"] };
	double z2 = { processedFile["geometry"]["step"]["z"] };
	double pt{ processedFile["geometry"]["powder thickness"] };
	bodySize = Vec3(x1, y1, z1);
	meshStep = Vec3(x2, y2, z2);
	powderThickness = pt;

	//time
	double st{ processedFile["time"]["start"] };
	double ts{ processedFile["time"]["step"] };
	double iter{ processedFile["time"]["iterations"] };
	startTime = st;
	timeStep = ts;
	endTime = timeStep * iter;

	// logging
	uint32_t dle{ processedFile["logging"]["log entries"] };
	desiredLogEntries = dle;

	// temperatures
	double at{ processedFile["temperatures"]["air"] };
	double it{ processedFile["temperatures"]["initial"] };
	double mt{ processedFile["temperatures"]["melting"] };
	airTemp = at;
	initialTemp = it;
	meltingTemp = mt;

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
	fusionEnthalpy = fe;
	solidRho = sr;
	packingRho = pr;
	liquidRho = lr;
	solidC = sc;
	liquidC = lc;
	solidKA = ska;
	solidKB = skb;
	liquidKA = lka;
	liquidKB = lkb;

	// radiant properties
	double emm{ processedFile["radiant properties"]["emmisivity"] };
	stefanBoltzmannConst = 5.67e-8;
	emmisivity = emm;

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
	laserVec = bodySize.dot(Vec3(x3, y3, z3));
	laserVel = Vec3(vx, vy, vz);
	laserRadius = r;
	laserPower = p;
	laserState = s;

	// calculated
	//const Vec3 laserVelScaled = laserVel.multiply(timeStep);
	sigmoidConst = 0.858148935;
	solidMass = meshStep.x * meshStep.y * meshStep.z * solidRho;
	liquidMass = meshStep.x * meshStep.y * meshStep.z * liquidRho;
	enthalpyMinus = solidMass * solidC * meltingTemp;
	enthalpyPlus = enthalpyMinus + solidMass * fusionEnthalpy;
	mscs = solidMass * solidC;
	mlcl = liquidMass * liquidC;
	mscsRev = 1 / mscs;
	mlclRev = 1 / mlcl;
	surfaceArea = meshStep.x * meshStep.y;
	radiantFluxConst = stefanBoltzmannConst * emmisivity;
	airTemp4 = airTemp * airTemp * airTemp * airTemp;
}

size_t Config::parallelProcesses = 0;
std::string Config::projectDir = "null";
Vec3 Config::bodySize = Vec3();
Vec3 Config::meshStep = Vec3();
double Config::powderThickness = 0.0;
double Config::startTime = 0.0;
double Config::timeStep = 0.0;
double Config::endTime = 0.0;
uint32_t Config::desiredLogEntries = 0;
double Config::airTemp = 0.0;
double Config::initialTemp = 0.0;
double Config::meltingTemp = 0.0;
double Config::fusionEnthalpy = 0.0;
double Config::solidRho = 0.0;
double Config::packingRho = 0.0;
double Config::liquidRho = 0.0;
double Config::solidC = 0.0;
double Config::liquidC = 0.0;
double Config::solidKA = 0.0;
double Config::solidKB = 0.0;
double Config::liquidKA = 0.0;
double Config::liquidKB = 0.0;
double Config::stefanBoltzmannConst = 0.0;
double Config::emmisivity = 0.0;
Vec3 Config::laserVec = Vec3();
Vec3 Config::laserVel = Vec3();
double Config::laserRadius = 0.0;
double Config::laserPower = 0.0;
bool Config::laserState = 0.0;
double Config::sigmoidConst = 0.0;
double Config::solidMass = 0.0;
double Config::liquidMass = 0.0;
double Config::enthalpyMinus = 0.0;
double Config::enthalpyPlus = 0.0;
double Config::mscs = 0.0;
double Config::mlcl = 0.0;
double Config::mscsRev = 0.0;
double Config::mlclRev = 0.0;
double Config::surfaceArea = 0.0;
double Config::radiantFluxConst = 0.0;
double Config::airTemp4 = 0.0;