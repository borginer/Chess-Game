cmake_minimum_required(VERSION 3.10)

project(ChessGame VERSION 1.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(raylib REQUIRED)

include_directories("${PROJECT_BINARY_DIR}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall")

# Specify the source directories
set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
set(ENGINE_DIR "${SRC_DIR}/engine")
set(INTERFACE_DIR "${SRC_DIR}/interface")

# find all .cpp files in engine and interface directories
file(GLOB ENGINE_SOURCES "${ENGINE_DIR}/*.cpp")
file(GLOB INTERFACE_SOURCES "${INTERFACE_DIR}/*.cpp")


set(ALL_SOURCES ${ENGINE_SOURCES} ${INTERFACE_SOURCES})

set(RAYLIB_INCLUDE_DIR "/opt/homebrew/include")
set(RAYLIB_LIB_DIR "/opt/homebrew/lib")

add_executable(chess ${ALL_SOURCES} src/main.cpp)
target_include_directories(chess PUBLIC "${SRC_DIR}" "${PROJECT_BINARY_DIR}"
    "${RAYLIB_INCLUDE_DIR}")
target_link_libraries(chess raylib)

find_package(GTest REQUIRED)
if(GTest_FOUND)
    enable_testing()

    add_executable(engine_tests ${ENGINE_SOURCES} src/tests.cpp)
    target_include_directories(engine_tests PUBLIC "${SRC_DIR}" "${PROJECT_BINARY_DIR}")
    target_link_libraries(engine_tests GTest::GTest GTest::Main)

    # adds whole exe as single test
    # add_test(NAME EngineTests COMMAND engine_tests)

    include(GoogleTest)
    gtest_discover_tests(engine_tests)
else()
    message(WARNING "Google Test not found. Not building tests")
endif()
