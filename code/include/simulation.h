#pragma once

#include <stdint.h>
#include <string>
#include "../../lib/include/timer.h"


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
	bool logThisStep;
	bool stopSimulation;
	Timer timer;

	SimulationIterator();
	~SimulationIterator();

	void advance();
	uint32_t digitsInValue(uint32_t x) const;
	std::string info();
};