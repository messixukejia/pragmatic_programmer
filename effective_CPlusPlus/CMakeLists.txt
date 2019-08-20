cmake_minimum_required(VERSION 3.9)
# Keep the version below in sync with the one in db.h
project(data_structrue)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(./lib/gtest-1.6.0)
enable_testing()

# Include the gtest library. gtest_SOURCE_DIR is available due to
# 'project(gtest)' above.
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

INCLUDE_DIRECTORIES(./common)
aux_source_directory(./common DIR_SRCS)
aux_source_directory(./data_structure DIR_SRCS)
add_executable(data_structure_test ${DIR_SRCS})

target_link_libraries(data_structure_test gtest gtest_main)