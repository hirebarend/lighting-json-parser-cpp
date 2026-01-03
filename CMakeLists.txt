cmake_minimum_required(VERSION 3.20)
project(lighting-json-parser-cpp CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(main src/main.cpp)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(main PRIVATE -march=native)
endif()

add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/run_output.txt
    COMMAND main
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    DEPENDS main
)

add_custom_target(
    run
    DEPENDS ${CMAKE_BINARY_DIR}/run_output.txt
)
