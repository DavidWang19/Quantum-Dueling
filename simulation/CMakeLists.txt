cmake_minimum_required(VERSION 3.22)
project(Quantum_Dueling)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")

add_executable(complexity_parameter_finder parameter_finder.cpp Dueling.cpp)
add_executable(complexity_data_generator data_generator.cpp Dueling.cpp)
add_executable(maximum_probability_all_M maximum_probability_all_M_original.cpp)
add_executable(iteration_probability iteration_probability_original.cpp)