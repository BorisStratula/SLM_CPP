#include <iostream>
#include <filesystem>
#include "../include/data_writer.h"
#include "../include/lean_vtk.hpp"

DataWriter::DataWriter() {
    projectDir = "C:/del/03";
    solutionDir = projectDir + "/solution";
    prepareDir();
}

void DataWriter::advance(SimulationIterator simIterator, BodyData bodyData) {
    uint32_t iteration = simIterator.iterationLogger;
    uint32_t totalIterations = simIterator.desiredLogEntries;
    std::string fileIndex = addPrefixZeroes(iteration, totalIterations);
    std::string fileName = "/data_" + fileIndex + ".vtu";
    writeSolutionFile(fileName, bodyData);
    combineSolutionFiles(iteration, totalIterations);
}

void DataWriter::prepareDir() {
    try {
        std::filesystem::remove_all(projectDir);
    }
    catch (...) {
        std::cout << "Close solution files first\n";
        std::exit(-1);
    }
    std::filesystem::create_directory(projectDir);
    std::filesystem::create_directory(solutionDir);
}

std::string DataWriter::addPrefixZeroes(uint32_t value, uint32_t maxValue) {
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

void DataWriter::writeSolutionFile(std::string fileName, BodyData bodyData) {
    Timer timer = Timer();
    timer.start();
    const int dim = 3;
    const int cell_size = 8;
    leanvtk::VTUWriter writer;

    writer.add_scalar_field("nodal ID", bodyData.nodalID);
    writer.add_cell_scalar_field("elem ID", bodyData.elemID);
    writer.add_cell_scalar_field("enthalpy", bodyData.H);
    writer.add_cell_scalar_field("temperature", bodyData.T);
    writer.add_cell_scalar_field("thermal conductivity", bodyData.k);
    writer.add_cell_scalar_field("under laser", bodyData.underLaser);
    writer.add_cell_scalar_field("laser specific powe", bodyData.qDebug);
    writer.add_cell_scalar_field("radiant flux", bodyData.MDebug);
    writer.add_cell_scalar_field("times melted", bodyData.timesMelted);
    writer.add_cell_scalar_field("state", bodyData.state);

    writer.write_volume_mesh(solutionDir + fileName, dim, cell_size, bodyData.nodalCoords, bodyData.elemVertices);
    std::cout << "writeSolutionFile " + timer.formatElapsed() + '\n';
}

void DataWriter::combineSolutionFiles(uint32_t iteration, uint32_t totalIterations) {
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