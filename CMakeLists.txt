cmake_minimum_required(VERSION 4.2)
project(LatticeEngine)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(engine)

add_executable(Sandbox sandbox/main.cpp)
target_link_libraries(Sandbox PUBLIC LatticeEngine)