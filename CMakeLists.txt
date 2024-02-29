# CMakeList.txt : CMake project for slm_cpp, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("slm_cpp")

set(SOURCES
	"main.cpp"
	"code/source/body_data.cpp"
	"code/source/data_writer.cpp"
	"code/source/element.cpp"
	"code/source/intvec3.cpp" 
	"code/source/laser.cpp" 
	"code/source/lean_vtk.cpp"
	"code/source/mesh.cpp" 
	"code/source/neighbours.cpp"
	"code/source/node.cpp" 
	"code/source/simulation.cpp"
	"code/source/timer.cpp"
	"code/source/vec3.cpp"
	)

# Add source to this project's executable.
add_executable (slm_cpp ${SOURCES})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET slm_cpp PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
