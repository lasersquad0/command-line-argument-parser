cmake_minimum_required (VERSION 3.27)

if(POLICY CMP0091)
    cmake_policy(SET CMP0091 NEW)
endif()

if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type")
endif()

#set(CMAKE_C_COMPILER gcc-13)  we dont use C compiler
set(CMAKE_CXX_COMPILER g++-13)

set(CLI_VER "0.0.2")
project("cli" VERSION ${CLI_VER} LANGUAGES CXX)

# define runtime library type - Threaded or Threaded DLL 
# use "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL" for Threaded DLL runtime type 
#if(MSVC)
#    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>" CACHE STRING "MSVC runtime library")
#endif()

# Generate and executable.
file(GLOB CLI_SRC CONFIGURE_DEPENDS "cli/*.cpp")
file(GLOB SANDBOX_SRC CONFIGURE_DEPENDS "sandbox/*.cpp")

# Generate cli sample executable.
add_executable(cli_sample ${CLI_SRC} ${SANDBOX_SRC})
#set_target_properties(cli_sample PROPERTIES OUTPUT_NAME "sample")

# Set minimal C++ version to C++11
#set_property(TARGET sandbox PROPERTY CXX_STANDARD 11)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set minimal C++ version to C++20
target_compile_features(cli_sample PRIVATE cxx_std_20)

# Add libraries include path.
target_include_directories(cli_sample PRIVATE "${PROJECT_SOURCE_DIR}/cli")

target_compile_options(cli_sample PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:
        -Wall
        -Wextra
        -Wpedantic
    >
    $<$<CXX_COMPILER_ID:MSVC>:
        /W4
        /permissive-
        /EHsc
    >
)

#if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    # Add "_DEBUG" definition to the source code.
#    target_compile_definitions(sandbox PRIVATE _DEBUG)
#else()
    # Add "NDEBUG" definition to the source code.
#    target_compile_definitions(sandbox PRIVATE NDEBUG)
#endif()
