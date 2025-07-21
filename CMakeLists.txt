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

# --- Add dependencies using FetchContent ---
include(FetchContent)

# cxxopts dependency
FetchContent_Declare(
  cxxopts
  GIT_REPOSITORY https://github.com/jarro2783/cxxopts.git
  GIT_TAG        v3.3.1
)

# GoogleTest dependency
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.17.0
)

FetchContent_MakeAvailable(cxxopts googletest)
# --- End of dependencies ---


# Define the main executable target
add_executable(wildcard_matcher
  src/main.cpp
)

# Add the 'include' directory to the target's include path
target_include_directories(wildcard_matcher PUBLIC "${PROJECT_SOURCE_DIR}/include")

# Link cxxopts to the main target
target_link_libraries(wildcard_matcher PRIVATE cxxopts::cxxopts)

# --- Setup Testing ---
# Enable testing for this project
enable_testing()

# Add a new executable for your tests
add_executable(run_tests
  test/test.cpp
)

# Add include directories for the test executable
# Main project headers
target_include_directories(run_tests PUBLIC "${PROJECT_SOURCE_DIR}/include")
# Test-specific headers
target_include_directories(run_tests PUBLIC "${PROJECT_SOURCE_DIR}/test/include")

# Link the test executable against GoogleTest and your project's code
target_link_libraries(run_tests PRIVATE GTest::gtest_main)

# Add the test to CTest
include(GoogleTest)
gtest_discover_tests(run_tests
  PROPERTIES TIMEOUT 60
)