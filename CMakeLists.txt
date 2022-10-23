cmake_minimum_required(VERSION 3.1)
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)

project(EmergencyButton LANGUAGES CXX C VERSION 1.0.0)
option(BUILD_PLUGIN "Build the EmergencyButtonPlugin" ON)

find_package(Threads REQUIRED)

add_subdirectory(emergency_button)

if(BUILD_PLUGIN)
  message(STATUS "Building with EmergencyButtonPlugin")
  add_subdirectory(plugin)
endif()
