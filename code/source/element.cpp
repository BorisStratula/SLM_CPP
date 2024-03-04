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
	vec = Config::meshStep.dot(INDEX_VECTOR);
	state = STATE;
	underLaser = 0;
	timesMelted = 0;
	wasProcessed = false;
	T = Config::initialTemp;
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
	if (state == 0) sigmoidConst = Config::sigmoidConst;
	if (T == Config::meltingTemp) {
		double a1 = Config::solidKA;
		double b1 = Config::solidKB;
		double a2 = Config::liquidKA;
		double b2 = Config::liquidKB;
		double ks = (a1 * T + b1) * sigmoidConst;
		double kl = a2 * T + b2;
		return ks + (kl - ks) * (H - Config::enthalpyMinus) / (Config::enthalpyPlus - Config::enthalpyMinus);
	}
	else {
		double a;
		double b;
		if (T < Config::meltingTemp) {
			a = Config::solidKA;
			b = Config::solidKB;
		}
		else {
			a = Config::liquidKA;
			b = Config::liquidKB;
		}
		return (a * T + b) * sigmoidConst;
	}
}

double Elem::TofH() const {
	if (H < Config::enthalpyMinus) return H * Config::mscsRev;
	else if (H > Config::enthalpyMinus and H <= Config::enthalpyPlus) return Config::meltingTemp;
	else return Config::meltingTemp + (H - Config::enthalpyPlus) * Config::mlclRev;
}

double Elem::HofT() const {
	if (T > Config::meltingTemp) return Config::mlcl * (T - Config::meltingTemp) + Config::enthalpyPlus;
	else return Config::mscs * T;
}

double Elem::enthalpyFlow() {
	wasProcessed = false;
	double thetaX = thetaI(neighboursTruncated.xPlus, neighboursTruncated.xMinus, 1, Elem::meshPtr);
	double thetaY = thetaI(neighboursTruncated.yPlus, neighboursTruncated.yMinus, 2, Elem::meshPtr);
	double thetaZ = thetaI(neighboursTruncated.zPlus, neighboursTruncated.zMinus, 3, Elem::meshPtr);
	double theta = Config::surfaceArea * (thetaX + thetaY + thetaZ);
	double q = 0;
	if (neighbours.zPlus == -1) q = Elem::laserPtr->heatToElem(this);
	qDebug = q;
	double M = radiantFlux();
	MDebug = M;
	double FExt = q - M;
	double enthalpyFlow = (theta + FExt)* Config::timeStep;
	return enthalpyFlow;
}

double Elem::thetaI(int32_t forwardID, int32_t backwardID, uint32_t axis, const Mesh* const MESH) const {
	double h;
	if (axis == 1) h = Config::meshStep.x;
	else if (axis == 2) h = Config::meshStep.y;
	else h = Config::meshStep.z;
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
	else return onSurface * Config::surfaceArea * Config::radiantFluxConst * (T * T * T * T - Config::airTemp4);
}

void Elem::chechState() {
	if (T > Config::meltingTemp) {
		if (state == 2 or state == 0) {
			state = 1;
			timesMelted += 1;
		}
	}
	if (T < Config::meltingTemp) {
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

