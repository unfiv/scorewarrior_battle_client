cmake_minimum_required(VERSION 3.27)
project(sw_battle_test)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp src/*.hpp)
add_executable(sw_battle_test ${SOURCES})

target_include_directories(sw_battle_test PUBLIC src/)

set_target_properties(sw_battle_test PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
        RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/Debug"
        RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/Release"
)

set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT sw_battle_test)

enable_testing()

add_test(
        NAME integration.snapshot.swordsman_deterministic
        COMMAND ${CMAKE_COMMAND}
        -DEXECUTABLE=$<TARGET_FILE:sw_battle_test>
        -DSCENARIO=${CMAKE_SOURCE_DIR}/tests/integration/scenarios/swordsman_deterministic.txt
        -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/integration/expected/swordsman_deterministic.log
        -P ${CMAKE_SOURCE_DIR}/tests/integration/cmake/AssertSnapshot.cmake
)
