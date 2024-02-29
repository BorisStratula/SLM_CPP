#include "../include/element.h"
#include "../include/config.h"
#include "../include/mesh.h"
#include "../include/laser.h"
#include "../include/intvec3.h"

//Elem::Elem() {
//
//}

Elem::Elem(uint32_t _ID, const IntVec3& INDEX_VECTOR, const Neighbours& NEIGHBOURS, const Neighbours& NEIGHBOURS_TRUNCATED, const std::string& STATE) {
	ID = _ID;
	vertices = new uint32_t[8];
	neighbours = NEIGHBOURS;
	neighboursTruncated = NEIGHBOURS_TRUNCATED;
	onSurface = neighboursTruncated.onSurface;
	vec = config::meshStep.dot(INDEX_VECTOR);
	T = config::initialTemp;
	state = STATE;
	underLaser = 0;
	timesMelted = 0;
	k = thermalConductivity();
	H = HofT();
	HFlow = 0;
	qDebug = 0;
	MDebug = 0;
}

Elem::~Elem() {
	// TODO fix the issue when destructor is called afterevery elem creation
	//delete[] vertices;
}

double Elem::thermalConductivity() const {
	double sigmoidConst = 1;
	if (state == "powder") sigmoidConst = config::sigmoidConst;
	if (T == config::meltingTemp) {
		double a1 = config::solidKA;
		double b1 = config::solidKB;
		double a2 = config::liquidKA;
		double b2 = config::liquidKB;
		double ks = (a1 * T + b1) * sigmoidConst;
		double kl = a2 * T + b2;
		return ks + (kl - ks) * (H - config::enthalpyMinus) / (config::enthalpyPlus - config::enthalpyMinus);
	}
	else {
		double a;
		double b;
		if (T < config::meltingTemp) {
			a = config::solidKA;
			b = config::solidKB;
		}
		else {
			a = config::liquidKA;
			b = config::liquidKB;
		}
		return (a * T + b) * sigmoidConst;
	}
}

double Elem::TofH() const {
	if (H < config::enthalpyMinus) return H * config::mscsRev;
	else if (H > config::enthalpyMinus and H <= config::enthalpyPlus) return config::meltingTemp;
	else return config::meltingTemp + (H - config::enthalpyPlus) * config::mlclRev;
}

double Elem::HofT() const {
	if (T > config::meltingTemp) return config::mlcl * (T - config::meltingTemp) + config::enthalpyPlus;
	else return config::mscs * T;
}

double Elem::enthalpyFlow(const Mesh* const MESH, const Laser* const LASER) {
	double thetaX = thetaI(neighboursTruncated.xPlus, neighboursTruncated.xMinus, 1, MESH);
	double thetaY = thetaI(neighboursTruncated.yPlus, neighboursTruncated.yMinus, 2, MESH);
	double thetaZ = thetaI(neighboursTruncated.zPlus, neighboursTruncated.zMinus, 3, MESH);
	double theta = config::surfaceArea * (thetaX + thetaY + thetaZ);
	double q = 0;
	if (neighbours.zPlus == -1) q = LASER->heatToElem(this);
	qDebug = q;
	double M = radiantFlux();
	MDebug = M;
	double FExt = q - M;
	double enthalpyFlow = (theta + FExt)*config::timeStep;
	return enthalpyFlow;
}

double Elem::thetaI(int32_t forwardID, int32_t backwardID, uint32_t axis, const Mesh* const MESH) const {
	double h;
	if (axis == 1) h = config::meshStep.x;
	else if (axis == 2) h = config::meshStep.y;
	else h = config::meshStep.z;
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
	else return onSurface * config::surfaceArea * config::radiantFluxConst * (T * T * T * T - config::airTemp4);
}

void Elem::chechState() {
	if (T > config::meltingTemp) {
		if (state == "solid" or state == "powder") {
			state = "liquid";
			timesMelted += 1;
		}
	}
	if (T < config::meltingTemp) {
		if (state == "liquid") {
			state = "solid";
		}
	}
}