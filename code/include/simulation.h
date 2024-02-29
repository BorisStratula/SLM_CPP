#pragma once

#include <stdint.h>
#include <string>
#include "timer.h"


class SimulationIterator {
public:
	double start;
	double end;
	double step;
	double now;
	uint32_t iteration;
	uint32_t iterationLogger;
	uint32_t totalIterations;
	uint32_t desiredLogEntries;
	uint32_t maxDigits;
	uint32_t logEvery;
	uint32_t logThisStep;
	uint32_t stopSimulation;
	Timer timer;

	SimulationIterator();
	~SimulationIterator();

	void advance();
	uint32_t valueLen(uint32_t x);
	std::string info();
};