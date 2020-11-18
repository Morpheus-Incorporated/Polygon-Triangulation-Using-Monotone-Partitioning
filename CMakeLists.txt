cmake_minimum_required(VERSION 3.10)

project(Polygon-Triangulation-Using-Monotone-Partitioning VERSION 1.0)

add_library(aPolygon-Triangulation-Using-Monotone-Partitioning INTERFACE)
target_include_directories(Polygon-Triangulation-Using-Monotone-Partitioning INTERFACE ${CMAKE_SOURCE_DIR}/Monotone)
target_compile_features(Polygon-Triangulation-Using-Monotone-Partitioning INTERFACE cxx_std_20)

add_subdirectory(src)
