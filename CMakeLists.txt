cmake_minimum_required(VERSION 3.28)
project(gve LANGUAGES CXX)

# C++ Standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# Find OpenMP
find_package(OpenMP)


# Interface library for C++17 (Header-only)
add_library(gve INTERFACE)
target_include_directories(gve INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:include>
)
target_compile_features(gve INTERFACE cxx_std_17)

if(OpenMP_CXX_FOUND)
    target_link_libraries(gve INTERFACE OpenMP::OpenMP_CXX)
endif()


# Module library for C++20
# Note: CMake support for C++ modules is still evolving.
# This target assumes a compiler that supports modules.
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "MSVC" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    add_library(gve_module)
    target_sources(gve_module PUBLIC
        FILE_SET CXX_MODULES FILES src/main.ixx
    )
    target_link_libraries(gve_module PUBLIC gve)
endif()
