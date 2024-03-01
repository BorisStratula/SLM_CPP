#pragma once

#include <string>


class Timer {
public:
	double timeCumulative;
	double timeOfPreviousIteration;
	double timeLeftPrecalc;
	double timeOfStart;
	uint32_t previousIterationNumber;

	Timer();
	~Timer();

	void start();
	void update();

	uint32_t today(double seconds) const;
	double timeMSK() const;
	double left(uint32_t iterationsNow, uint32_t iterationTotal);

	std::string formatElapsed();
	std::string formatLeft(uint32_t iterationsNow, uint32_t iterationsTotal);
	std::string formatETA(uint32_t iteratiosnNow, uint32_t iterationsTotal);
	std::string formatTime(double seconds) const;
};