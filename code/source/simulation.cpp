#include <iostream>
#include <format>
#include "../include/simulation.h"
#include "../include/config.h"

SimulationIterator::SimulationIterator() {
	start = config::startTime;
	end = config::endTime;
	step = config::timeStep;
	now = start;
	iteration = 0;
	iterationLogger = 0;
	totalIterations = (uint32_t)std::round((end - start) / step);
	desiredLogEntries = config::desiredLogEntries - 1;
	maxDigits = valueLen(desiredLogEntries);
	logEvery = (uint32_t)round((double)totalIterations / (double)desiredLogEntries);
	logThisStep = 0;
	stopSimulation = 0;
	timer.start();
}

SimulationIterator::~SimulationIterator() {

}

void SimulationIterator::advance() {
	now += step;
	iteration += 1;
	if (iteration % logEvery == 0) {
		logThisStep = 1;
		iterationLogger += 1;
		std::cout << info() << '\n';
		if (iterationLogger == desiredLogEntries) {
			stopSimulation = 1;
		}
	}
	else {
		logThisStep = 0;
	}
}

uint32_t SimulationIterator::valueLen(uint32_t x) {
	std::string str = std::to_string(x);
	uint32_t length = 0;
	size_t iMax = str.length();
	for (size_t i = 0; i < iMax; i++) {
		length += 1;
	}
	return length;
}

std::string SimulationIterator::info() {
	std::string iterationsOutOfTotal = std::to_string(iterationLogger);
	size_t strLen = iterationsOutOfTotal.length();
	while (strLen < maxDigits) {
		iterationsOutOfTotal = ' ' + iterationsOutOfTotal;
		strLen += 1;
	}
	iterationsOutOfTotal += " / " + std::to_string(desiredLogEntries);
	std::string elapsed = timer.formatElapsed();
	std::string left = timer.formatLeft(iterationLogger, desiredLogEntries);
	std::string eta = timer.formatETA(iterationLogger, desiredLogEntries);	
	std::string string = iterationsOutOfTotal + " | " + elapsed + " | " + left + " | " + eta;
	return string;
}