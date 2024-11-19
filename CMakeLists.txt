cmake_minimum_required(VERSION 3.10)

project(ChessGame VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

find_package(raylib REQUIRED)
find_package(websocketpp REQUIRED)
find_package(nlohmann_json 3.11.2 REQUIRED)
cmake_policy(SET CMP0167 NEW)
find_package(Boost REQUIRED system thread)

include_directories("${PROJECT_BINARY_DIR}" websocketpp)
# add -O3 later, g0 to not generate debug symbols
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -g0") 
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl")

# source dirs
set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
set(ENGINE_DIR "${SRC_DIR}/engine")
set(INTERFACE_DIR "${SRC_DIR}/interface")
set(NETWORK_DIR "${SRC_DIR}/network")

# find all .cpp files in engine and interface directories
file(GLOB ENGINE_SOURCES "${ENGINE_DIR}/*.cpp")
file(GLOB INTERFACE_SOURCES "${INTERFACE_DIR}/*.cpp")
file(GLOB NETWORK_SOURCES "${NETWORK_DIR}/*.cpp")


set(ALL_SOURCES ${ENGINE_SOURCES} ${INTERFACE_SOURCES})

set(RAYLIB_INCLUDE_DIR "/opt/homebrew/include")
set(RAYLIB_LIB_DIR "/opt/homebrew/lib")
set(WEBSOCKETPP_INCLUDE_DIR "/opt/homebrew/include")

add_executable(chess ${ALL_SOURCES} src/main.cpp)
target_include_directories(chess PUBLIC ${SRC_DIR} ${PROJECT_BINARY_DIR}
    ${RAYLIB_INCLUDE_DIR})
target_link_libraries(chess raylib)


find_package(GTest REQUIRED)
if(GTest_FOUND)
    enable_testing()

    add_executable(engine_tests ${ENGINE_SOURCES} src/tests.cpp)
    target_include_directories(engine_tests PUBLIC ${SRC_DIR} ${PROJECT_BINARY_DIR})
    target_link_libraries(engine_tests GTest::GTest GTest::Main)

    # adds whole exe as single test
    # add_test(NAME EngineTests COMMAND engine_tests)

    include(GoogleTest)
    gtest_discover_tests(engine_tests)
else()
    message(WARNING "Google Test not found. Not building tests")
endif()


add_executable(game_server ${ENGINE_SOURCES} src/network/game_prot.cpp 
    src/network/game_server.cpp)
target_include_directories(game_server PUBLIC ${ENGINE_DIR} 
    ${WEBSOCKETPP_INCLUDE_DIR} ${PROJECT_BINARY_DIR})

find_package(Boost REQUIRED COMPONENTS system)
target_link_libraries(game_server Boost::system 
    pthread nlohmann_json::nlohmann_json)


add_executable(game_client ${ENGINE_SOURCES} ${INTERFACE_SOURCES} 
    src/network/game_prot.cpp src/network/game_client.cpp)
target_include_directories(game_client PUBLIC ${SRC_DIR} 
    ${WEBSOCKETPP_INCLUDE_DIR} ${PROJECT_BINARY_DIR} ${RAYLIB_INCLUDE_DIR})

find_package(Boost REQUIRED COMPONENTS system)
target_link_libraries(game_client Boost::system 
    pthread nlohmann_json::nlohmann_json raylib)
