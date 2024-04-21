cmake_minimum_required(VERSION 3.26)
project("Hangman")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)

include_directories(
  "${CMAKE_SOURCE_DIR}/src/"
)

add_subdirectory("${CMAKE_SOURCE_DIR}/src")

