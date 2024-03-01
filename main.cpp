#include "main.h"

int main()
{
	printf("Calculation process has started\n");
	auto simulation = SimulationIterator();
	auto laser = Laser();
	auto mesh = Mesh();
	auto bodyData = BodyData(&mesh);
	auto dataWriter = DataWriter();
	dataWriter.advance(simulation, bodyData);

	while (simulation.stopSimulation != 1) {
		simulation.advance();
		mesh.advance(&laser, &bodyData);
		laser.advance();

		//if (bodyData.TMax > 1960.0) laser.state = "off";

		if (simulation.logThisStep == 1) {
			bodyData.advance(&mesh);
			dataWriter.advance(simulation, bodyData);
		}
	}
	printf("Calculation process has started\n");
	return 0;
}
