################################################################################
# Top-level build file for SIFT3D QIFP module.
################################################################################

# Minimum CMake version
cmake_minimum_required (VERSION 2.8)

# Project name
project (sift3d-internal)

# Paths 
list (APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)
list (APPEND CMAKE_MODULE_PATH /usr/local/lib/cmake/sift3d)

# Output directories
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Build flags
set (DEBUG_FLAGS "-g -DVERBOSE")
set (RELEASE_FLAGS "-O3 -DNDEBUG")

# GCC-specific flags
if (CMAKE_C_COMPILER_ID STREQUAL "GNU" OR 
    CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set (DEBUG_FLAGS "${DEBUG_FLAGS} -ggdb3")
    set (RELEASE_FLAGS "${RELEASE_FLAGS} -ffast-math")
endif ()

set (CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} ${DEBUG_FLAGS}")
set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${DEBUG_FLAGS}")
set (CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} ${RELEASE_FLAGS}")
set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${RELEASE_FLAGS}")

# OS-specific build flags
if (APPLE)
        set(CMAKE_MACOSX_RPATH ON)
endif()

# Find SIFT3D
find_package (SIFT3D)

# Add the executable
add_executable (sift3d_qifp sift3d_main.c miniball.cpp)
target_link_libraries (sift3d_qifp PUBLIC ${SIFT3D_LIBRARIES})
target_include_directories (sift3d_qifp PUBLIC ${SIFT3D_INCLUDE_DIRS})
