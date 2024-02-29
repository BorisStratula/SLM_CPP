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

	void prepareDir() const;
	void advance(const SimulationIterator& SIMULATION, const BodyData& BODY_DATA) const;
	void writeSolutionFile(const std::string& FILE_NAME, const BodyData& BODY_DATA) const;
	void combineSolutionFiles(uint32_t iteration, uint32_t totalIterations) const;
	std::string addPrefixZeroes(uint32_t value, uint32_t maxValue) const;
};