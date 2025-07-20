# CMake minimum version required to build this project
cmake_minimum_required(VERSION 3.15)

# Define the project name and language
project(wildcard_matcher LANGUAGES CXX)

# Generate a 'compile_commands.json' file in the build directory
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Define the executable target and specify its source files
add_executable(wildcard_matcher
    src/main.cpp
)

# Add the 'include' directory to the target's include path
target_include_directories(wildcard_matcher PUBLIC "${PROJECT_SOURCE_DIR}/include")