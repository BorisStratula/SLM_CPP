#pragma once

#include <string>
#include "body_data.h"
#include "simulation.h"

class DataWriter {
public:
	std::string projectDir;
	std::string solutionDir;

	DataWriter();
	~DataWriter() = default;

	void prepareDir();
	void advance(SimulationIterator simIterator, BodyData bodyData);
	void writeSolutionFile(std::string fileName, BodyData bodyData);
	void combineSolutionFiles(uint32_t iteration, uint32_t totalIterations);
	std::string addPrefixZeroes(uint32_t value, uint32_t maxValue);
};