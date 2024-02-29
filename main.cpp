#include "main.h"

// for debug purpose
#include <chrono>
#include <thread>
#include <vector>
#include <filesystem>
#include <fstream>
#include <iostream>
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

	// mesh destructor debugging process
	//for (uint32_t i = 0; i < 8; i++) {
	//	std::cout << mesh.elems[0].vertices[i] << " ";
	//}
	//std::cout << "\n";
	//return 1;

	while (simulation.stopSimulation != 1) {
		simulation.advance();
		mesh.advance(&laser, &bodyData);
		laser.advance();

		if (bodyData.TMax > 1960.0) laser.state = "off";

		if (simulation.logThisStep == 1) {
			bodyData.advance(&mesh);
			dataWriter.advance(simulation, bodyData);
		}
	}
	//printf("We have reached the end");
	return 0;
}
