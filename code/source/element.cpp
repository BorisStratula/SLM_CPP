#include "../../lib/include/intvec3.h"
#include "../include/element.h"
#include "../include/config.h"
#include "../include/mesh.h"
#include "../include/laser.h"

Mesh* Elem::meshPtr = nullptr;
Laser* Elem::laserPtr = nullptr;

Elem::~Elem() {
	if (vertices) {
		delete[] vertices;
		vertices = nullptr;
	}
}

bool Elem::init(uint32_t _ID, const IntVec3& INDEX_VECTOR, const Neighbours& NEIGHBOURS, const Neighbours& NEIGHBOURS_TRUNCATED, const uint32_t STATE) {
	if (vertices != nullptr) return false;
	ID = _ID;
	vertices = new uint32_t[8];
	neighbours = NEIGHBOURS;
	neighboursTruncated = NEIGHBOURS_TRUNCATED;
	onSurface = neighboursTruncated.onSurface;
	vec = Config::Geometry::step.dot(INDEX_VECTOR);
	state = STATE;
	underLaser = 0;
	timesMelted = 0;
	wasProcessed = false;
	T = Config::Temperature::initial;
	k = thermalConductivity();
	H = HofT();
	HFlow = 0.0;
	qDebug = 0.0;
	MDebug = 0.0;
	return true;
}

bool Elem::valid() const {
	return vertices != nullptr;
}

double Elem::thermalConductivity() const {
	double sigmoidConst = 1;
	if (state == 0) sigmoidConst = Config::Misc::sigmoidConst;
	if (T == Config::Temperature::melting) {
		double a1 = Config::Energy::Solid::KA;
		double b1 = Config::Energy::Solid::KB;
		double a2 = Config::Energy::Liquid::KA;
		double b2 = Config::Energy::Liquid::KB;
		double ks = (a1 * T + b1) * sigmoidConst;
		double kl = a2 * T + b2;
		return ks + (kl - ks) * (H - Config::Energy::Enthalpy::minus) / (Config::Energy::Enthalpy::plus - Config::Energy::Enthalpy::minus);
	}
	else {
		double a;
		double b;
		if (T < Config::Temperature::melting) {
			a = Config::Energy::Solid::KA;
			b = Config::Energy::Solid::KB;
		}
		else {
			a = Config::Energy::Liquid::KA;
			b = Config::Energy::Liquid::KB;
		}
		return (a * T + b) * sigmoidConst;
	}
}

double Elem::TofH() const {
	if (H < Config::Energy::Enthalpy::minus) return H * Config::Energy::Solid::mcRev;
	else if (H > Config::Energy::Enthalpy::minus and H <= Config::Energy::Enthalpy::plus) return Config::Temperature::melting;
	else return Config::Temperature::melting + (H - Config::Energy::Enthalpy::plus) * Config::Energy::Liquid::mcRev;
}

double Elem::HofT() const {
	if (T > Config::Temperature::melting) return Config::Energy::Liquid::mc * (T - Config::Temperature::melting) + Config::Energy::Enthalpy::plus;
	else return Config::Energy::Solid::mc * T;
}

double Elem::enthalpyFlow() {
	wasProcessed = false;
	double thetaX = thetaI(neighboursTruncated.xPlus, neighboursTruncated.xMinus, 1, Elem::meshPtr);
	double thetaY = thetaI(neighboursTruncated.yPlus, neighboursTruncated.yMinus, 2, Elem::meshPtr);
	double thetaZ = thetaI(neighboursTruncated.zPlus, neighboursTruncated.zMinus, 3, Elem::meshPtr);
	double theta = Config::Geometry::surfaceArea * (thetaX + thetaY + thetaZ);
	double q = 0.0;
	double sideWallsCoolingRate = 0.0;
	double sideWallFlowRestrictor = 1.0;
	if (neighbours.zPlus == -1) {
		q = Elem::laserPtr->heatToElem(this);
	}
	//if ((neighbours.zMinus == -1) or (neighbours.xMinus == -1) or (neighbours.xPlus == -1) or (neighbours.yMinus == -1) or (neighbours.yPlus == -1)) {
	//	//sideWallsCoolingRate = Config::coolingPowerPerNode * onSurface;
	//	sideWallFlowRestrictor = 0.0;
	//}
	qDebug = q;
	double M = radiantFlux();
	MDebug = M;
	double FExt = q - M - sideWallsCoolingRate;
	double enthalpyFlow = (theta + FExt) * Config::Time::step * sideWallFlowRestrictor;
	return enthalpyFlow;
}

double Elem::thetaI(int32_t forwardID, int32_t backwardID, uint32_t axis, const Mesh* const MESH) const {
	double h;
	if (axis == 1) h = Config::Geometry::step.x;
	else if (axis == 2) h = Config::Geometry::step.y;
	else h = Config::Geometry::step.z;
	double divider = 2 * h * h;
	return (thetaF(forwardID, MESH) - thetaB(backwardID, MESH)) / divider;
}

double Elem::thetaF(int32_t forwardID, const Mesh* const MESH) const {
	return (MESH->elems[forwardID].k + k) * (MESH->elems[forwardID].T - T);
}

double Elem::thetaB(int32_t backwardID, const Mesh* const MESH) const {
	return (k + MESH->elems[backwardID].k) * (T - MESH->elems[backwardID].T);
}

double Elem::radiantFlux() const {
	if (onSurface == 0) return 0;
	else return onSurface * Config::Geometry::surfaceArea * Config::Radiation::fluxConst * (T * T * T * T - Config::Temperature::air4);
}

void Elem::chechState() {
	if (T > Config::Temperature::melting) {
		if (state == 2 or state == 0) {
			state = 1;
			timesMelted += 1;
		}
	}
	if (T < Config::Temperature::melting) {
		if (state == 1) {
			state = 2;
		}
	}
}

void Elem::calcStep1() {
	wasProcessed = false;
	HFlow = enthalpyFlow();
}

void Elem::calcStep2() {
	if (!wasProcessed) {
		wasProcessed = true;
		H += HFlow;
		T = TofH();
		k = thermalConductivity();
		chechState();
	}
}

