cmake_minimum_required (VERSION 3.27)
project("cli" VERSION 0.0.1 LANGUAGES CXX)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Default build type: Debug" FORCE)
endif()

# Generate and executable.
file(GLOB_RECURSE CLI_SRC "*cli/*.h" "cli/*.cpp")
file(GLOB_RECURSE SANDBOX_SRC "sandbox/*.h" "sandbox/*.cpp")

# Generate the test executable.
add_executable(sandbox ${CLI_SRC} ${SANDBOX_SRC})

# Set minimal C++ version to C++11
set_property(TARGET sandbox PROPERTY CXX_STANDARD 11)

# Add libraries include path.
target_include_directories(sandbox PRIVATE "${CMAKE_SOURCE_DIR}/cli")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    # Add "_DEBUG" definition to the source code.
    target_compile_definitions(sandbox PRIVATE _DEBUG)
else()
    # Add "NDEBUG" definition to the source code.
    target_compile_definitions(sandbox PRIVATE NDEBUG)
endif()
