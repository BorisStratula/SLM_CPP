#pragma once

#include <stdio.h>

#include "lib/include/Process.h"

#include "code/include/simulation.h"
#include "code/include/laser.h"
#include "code/include/mesh.h"
#include "code/include/body_data.h"
#include "code/include/data_writer.h"
#include "code/include/config.h"

/*
EXIT CODES
1 = DataWriter::prepareDir
2 = Mesh::createElement
3 = Config::readConfig
4 = DataWriter::prepareDir
5 = Config::readConfig
*/