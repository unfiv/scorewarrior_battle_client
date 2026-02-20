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

# Define assets dirs
set(ASSETS_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/assets")
set(ASSETS_TARGET_DIR "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets")

# Copy whole assets dir each build (with cleanup)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E rm -rf "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets"
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${ASSETS_SOURCE_DIR}" "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets"
    COMMENT "Cleaning and copying assets to: $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets"
)

# Integration tests
enable_testing()

foreach(scenario_index IN ITEMS 1 2 3 4)
    foreach(chance IN ITEMS 0 100)
        add_test(
            NAME integration.snapshot.scenario_${scenario_index}_chances_${chance}
            COMMAND ${CMAKE_COMMAND}
                -DEXECUTABLE=$<TARGET_FILE:sw_battle_test>
                -DSCENARIO=${CMAKE_SOURCE_DIR}/tests/integration/scenarios/scenario_${scenario_index}_chances_${chance}.txt
                -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/integration/expected/scenario_${scenario_index}_chances_${chance}.log
                -P ${CMAKE_SOURCE_DIR}/tests/integration/cmake/AssertSnapshot.cmake
        )
    endforeach()
endforeach()

