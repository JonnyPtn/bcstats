cmake_minimum_required(VERSION 3.9)

project(bcstats)

# We're using c++17
set (CMAKE_CXX_STANDARD 17)

# Add source files
add_subdirectory(src)

# Create the main executable
add_executable(bcstats src/main.cpp ${PROJECT_SRC})
target_include_directories(bcstats PUBLIC include)

# Create the test executable
add_executable(bctest tests/tests.cpp ${PROJECT_SRC})
target_include_directories(bctest PUBLIC include src)

# Set up CMake to execute tests
enable_testing()
add_test(NAME Catch COMMAND bctest)