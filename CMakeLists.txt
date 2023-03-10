cmake_minimum_required(VERSION 3.24)
project(quaternions)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB SOURCES
    ${PROJECT_SOURCE_DIR}/${PROJECT_NAME}/*.h
    ${PROJECT_SOURCE_DIR}/${PROJECT_NAME}/*.cpp)
list(FILTER SOURCES EXCLUDE REGEX ".test.cpp")
add_library(quaternions STATIC ${SOURCES})

include(FetchContent)
message(STATUS "Fetching Catch2 library ...")

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.3.2
)

FetchContent_MakeAvailable(Catch2)

file(GLOB TEST_SOURCES ${PROJECT_SOURCE_DIR}/${PROJECT_NAME}/*.test.cpp)
add_executable(tests ${TEST_SOURCES})
target_link_libraries(tests
    PRIVATE quaternions
    PRIVATE Catch2::Catch2WithMain)

include(CTest)
include(Catch)
catch_discover_tests(tests)