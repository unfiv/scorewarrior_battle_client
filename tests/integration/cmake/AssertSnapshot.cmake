cmake_minimum_required(VERSION 3.27)

if(NOT DEFINED EXECUTABLE)
    message(FATAL_ERROR "EXECUTABLE is required")
endif()
if(NOT DEFINED SCENARIO)
    message(FATAL_ERROR "SCENARIO is required")
endif()
if(NOT DEFINED EXPECTED)
    message(FATAL_ERROR "EXPECTED is required")
endif()

execute_process(
        COMMAND "${EXECUTABLE}" "${SCENARIO}"
        RESULT_VARIABLE result
        OUTPUT_VARIABLE actual_output
        ERROR_VARIABLE stderr_output
)

if(NOT result EQUAL 0)
    message(FATAL_ERROR "Simulation failed with code ${result}\n${stderr_output}")
endif()

file(READ "${EXPECTED}" expected_output)

if(NOT actual_output STREQUAL expected_output)
    set(actual_path "${CMAKE_CURRENT_BINARY_DIR}/actual_output.log")
    file(WRITE "${actual_path}" "${actual_output}")
    message(FATAL_ERROR
            "Snapshot mismatch\nScenario: ${SCENARIO}\nExpected: ${EXPECTED}\nActual: ${actual_path}")
endif()
