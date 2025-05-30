# CMakeList.txt : CMake project for Drawing_Spells_Simulator, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (Drawing_Spells_Simulator "Drawing_Spells_Simulator.cpp" "Drawing_Spells_Simulator.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Drawing_Spells_Simulator PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
