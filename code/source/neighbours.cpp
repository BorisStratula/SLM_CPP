#include "../include/neighbours.h"
#include "../include/vec3.h"


//Neighbours::Neighbours() {
//	xMinus = 0;
//	xPlus = 0;
//	yMinus = 0;
//	yPlus = 0;
//	zMinus = 0;
//	zPlus = 0;
//	origin = 0;
//	onSurface = 0;
//}


Neighbours::Neighbours(IntVec3 indexVec, uint32_t ID, IntVec3 resolution) {
	xMinus = xMinusID(indexVec, ID);
	xPlus = xPlusID(indexVec, ID, resolution);
	yMinus = yMinusID(indexVec, ID, resolution);
	yPlus = yPlusID(indexVec, ID, resolution);
	zMinus = zMinusID(indexVec, ID, resolution);
	zPlus = zPlusID(indexVec, ID, resolution);
	origin = (int32_t)ID;
	onSurface = 0;
}


Neighbours::~Neighbours() {

}


int32_t Neighbours::xMinusID(IntVec3 indexVec, uint32_t ID) {
	if (indexVec.x == 0) {
		return -1;
	}
	else {
		return ID - 1;
	}
}

int32_t Neighbours::xPlusID(IntVec3 indexVec, uint32_t ID, IntVec3 resolution) {
	if (indexVec.x == resolution.x - 1) {
		return -1;
	}
	else {
		return ID + 1;
	}
}

int32_t Neighbours::yMinusID(IntVec3 indexVec, uint32_t ID, IntVec3 resolution) {
	if (indexVec.y == 0) {
		return -1;
	}
	else {
		return ID - resolution.x;
	}
}

int32_t Neighbours::yPlusID(IntVec3 indexVec, uint32_t ID, IntVec3 resolution) {
	if (indexVec.y == resolution.y - 1) {
		return -1;
	}
	else {
		return ID + resolution.x;
	}
}

int32_t Neighbours::zMinusID(IntVec3 indexVec, uint32_t ID, IntVec3 resolution) {
	if (indexVec.z == 0) {
		return -1;
	}
	else {
		return ID - resolution.x * resolution.y;
	}
}

int32_t Neighbours::zPlusID(IntVec3 indexVec, uint32_t ID, IntVec3 resolution) {
	if (indexVec.z == resolution.z - 1) {
		return -1;
	}
	else {
		return ID + resolution.x * resolution.y;
	}
}


void Neighbours::truncate() {
	if (xMinus == -1) {
		xMinus = origin;
		onSurface += 1;
	}
	if (xPlus == -1) {
		xPlus = origin;
		onSurface += 1;
	}
	if (yMinus == -1) {
		yMinus = origin;
		onSurface += 1;
	}
	if (yPlus == -1) {
		yPlus = origin;
		onSurface += 1;
	}
	if (zMinus == -1) {
		zMinus = origin;
		onSurface += 1;
	}
	if (zPlus == -1) {
		zPlus = origin;
		onSurface += 1;
	}
}