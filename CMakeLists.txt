cmake_minimum_required(VERSION 2.8)
project(main)
set(CMAKE_BUILD_TYPE DEBUG)

include(CTest)
enable_testing()

link_directories(~/lib/systemc-2.3.3/lib-linux64)
include_directories(~/lib/systemc-2.3.3/include)
add_executable(main main.cpp)
target_link_libraries(${PROJECT_NAME} -lsystemc)