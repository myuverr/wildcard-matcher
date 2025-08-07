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
include(GoogleTest)

# --- Parser Tests ---
add_executable(run_parser_tests
  test/test_parser.cpp
)
target_include_directories(run_parser_tests PUBLIC
  "${PROJECT_SOURCE_DIR}/include"
  "${PROJECT_SOURCE_DIR}/test/include"
)
target_link_libraries(run_parser_tests PRIVATE GTest::gtest_main)
gtest_discover_tests(run_parser_tests
  PROPERTIES
    LABELS "parser"
)

# --- Validator Tests ---
add_executable(run_validator_tests
  test/test_validator.cpp
)
target_include_directories(run_validator_tests PUBLIC
  "${PROJECT_SOURCE_DIR}/include"
  "${PROJECT_SOURCE_DIR}/test/include"
)
target_link_libraries(run_validator_tests PRIVATE GTest::gtest_main)
gtest_discover_tests(run_validator_tests
  PROPERTIES
    LABELS "validator"
)

# --- Solvers Tests ---
add_executable(run_solvers_tests
  test/test_solvers.cpp
)
target_include_directories(run_solvers_tests PUBLIC
  "${PROJECT_SOURCE_DIR}/include"
  "${PROJECT_SOURCE_DIR}/test/include"
)
target_link_libraries(run_solvers_tests PRIVATE GTest::gtest_main)
gtest_discover_tests(run_solvers_tests
  PROPERTIES
    LABELS "solvers"
)