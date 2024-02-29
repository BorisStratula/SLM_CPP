#include "main.h"

// for debug purpose
#include <chrono>
#include <thread>
#include <vector>
#include <filesystem>
#include <fstream>
#include "code/include/vec3.h"
//


int main()
{
	printf("Calculation process has started\n");
	auto simulation = SimulationIterator();
	auto laser = Laser();
	auto mesh = Mesh();
	auto bodyData = BodyData(&mesh);
	auto dataWriter = DataWriter();
	dataWriter.advance(simulation, bodyData);
	std::vector<double> T = std::vector<double>();

	while (simulation.stopSimulation != 1) {
		simulation.advance();
		mesh.advance(&laser);
		bodyData.advance(&mesh);
		laser.advance();

		if (bodyData.TMax > 1960.0) laser.state = "off";

		if (simulation.logThisStep == 1) dataWriter.advance(simulation, bodyData);
	}

	return 0;
}
