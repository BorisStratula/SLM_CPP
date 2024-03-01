#include <iostream>
#include <filesystem>
#include "../../lib/include/lean_vtk.hpp"
#include "../include/data_writer.h"
#include "../include/config.h"

DataWriter::DataWriter() {
    projectDir = config::projectDir;
    solutionDir = projectDir + "/solution";
    prepareDir();
}

void DataWriter::advance(const SimulationIterator& SIMULATION, const BodyData& BODY_DATA) const {
    uint32_t iteration = SIMULATION.iterationLogger;
    uint32_t totalIterations = SIMULATION.desiredLogEntries;
    std::string fileIndex = addPrefixZeroes(iteration, totalIterations);
    std::string fileName = "/data_" + fileIndex + ".vtu";
    writeSolutionFile(fileName, BODY_DATA);
    combineSolutionFiles(iteration, totalIterations);
}

void DataWriter::prepareDir() const {
    try {
        std::filesystem::remove_all(projectDir);
    }
    catch (...) {
        std::cout << "Close solution files first\n";
        std::exit(1);
    }
    std::filesystem::create_directory(projectDir);
    std::filesystem::create_directory(solutionDir);
}

std::string DataWriter::addPrefixZeroes(uint32_t value, uint32_t maxValue) const {
    std::string str;
    uint32_t valueDigits = (uint32_t)std::to_string(value).size();
    uint32_t maxValueDigits = (uint32_t)std::to_string(maxValue).size();
    uint32_t deltaOfDigits = maxValueDigits - valueDigits;
    for (uint32_t i = 0; i < deltaOfDigits; i++) {
        str += "0";
    }
    str += std::to_string(value);
    return str;
}

void DataWriter::writeSolutionFile(const std::string& FILE_NAME, const BodyData& BODY_DATA) const {
    Timer timer = Timer();
    timer.start();
    const int dim = 3;
    const int cell_size = 8;
    leanvtk::VTUWriter writer;

    writer.add_scalar_field("nodal ID", BODY_DATA.nodalID);
    writer.add_cell_scalar_field("elem ID", BODY_DATA.elemID);
    writer.add_cell_scalar_field("enthalpy", BODY_DATA.H);
    writer.add_cell_scalar_field("temperature", BODY_DATA.T);
    writer.add_cell_scalar_field("thermal conductivity", BODY_DATA.k);
    writer.add_cell_scalar_field("under laser", BODY_DATA.underLaser);
    writer.add_cell_scalar_field("laser specific powe", BODY_DATA.qDebug);
    writer.add_cell_scalar_field("radiant flux", BODY_DATA.MDebug);
    writer.add_cell_scalar_field("times melted", BODY_DATA.timesMelted);
    writer.add_cell_scalar_field("state", BODY_DATA.state);

    writer.write_volume_mesh(solutionDir + FILE_NAME, dim, cell_size, BODY_DATA.nodalCoords, BODY_DATA.elemVertices);
    std::cout << "writeSolutionFile " + timer.formatElapsed() + '\n';
}

void DataWriter::combineSolutionFiles(uint32_t iteration, uint32_t totalIterations) const {
    std::string str;
    std::string number;
    str  = "<?xml version=\"1.0\"?>\n";
    str += "<VTKFile type=\"Collection\" byte_order=\"LittleEndian\">\n";
    str += "  <Collection>\n";
    for (uint32_t entry = 0; entry < (iteration + 1); entry++) {
        number = addPrefixZeroes(entry, totalIterations);
        str += std::format("    <DataSet step=\"{}\" part=\"0\" file=\"solution/data_{}.vtu\" name=\"Asmb: Part: Matl:ELASTIC\"/>\n", entry, number);
    }
    str += "  </Collection>\n";
    str += "</VTKFile>";
    std::ofstream combinedSolutionFile(projectDir + "/solution.pvd");
    combinedSolutionFile << str;
    combinedSolutionFile.close();
}