#include "main.h"

int main()
{
	printf("///////////////////////////////////////////////////////////////\n");
	printf("// Selective Laser Melting (SLM) numerical modelling program //\n");
	printf("///////////////////////////////////////////////////////////////\n");
	Config::readConfig();
	auto simulation = SimulationIterator();
	auto laser = Laser();
	auto mesh = Mesh(&laser);
	auto bodyData = BodyData(&mesh);
	auto dataWriter = DataWriter();
	dataWriter.advance(simulation, bodyData);


	while (!simulation.stopSimulation) {
		simulation.advance();
		mesh.advance();
		laser.advance();

		if (bodyData.TMax > Config::Temperature::cutOff) laser.state = false;

		if (simulation.logThisStep) {
			bodyData.advance(&mesh);
			dataWriter.advance(simulation, bodyData);
		}
	}


	printf("//////////////////////////\n");
	printf("// Program has finished //\n");
	printf("//////////////////////////\n");
	return 0;
}
