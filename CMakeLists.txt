cmake_minimum_required(VERSION 3.20)
project(lighting-json-parser-cpp CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)
FetchContent_Declare(
  simdjson
  GIT_REPOSITORY https://github.com/simdjson/simdjson.git
  GIT_TAG v3.1.0 # Using a specific version for stability
)
FetchContent_MakeAvailable(simdjson)

add_executable(main src/main.cpp)

target_include_directories(main PRIVATE third_party)

target_link_libraries(main PRIVATE simdjson)

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
