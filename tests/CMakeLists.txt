# CMake version string
cmake_minimum_required(VERSION 3.0)

if (WIN32)
    remove_definitions(-DLINKS_DLL)
endif ()

# Project
project(test_generic_repository)

# Output
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/tests)

if(MSVC)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/result/Release/tests)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/result/Debug/tests)
endif(MSVC)

# Enable Qt modules
find_package(Qt5 COMPONENTS Test REQUIRED)

# Test Sources
file(GLOB_RECURSE TEST_SOURCES "*.h" "*.cpp")

# Executable
add_executable(${PROJECT_NAME} ${TEST_SOURCES})

# Includes
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Link Libraries
target_link_libraries(${PROJECT_NAME} generic_sql Qt5::Test)

# Tests
add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})
