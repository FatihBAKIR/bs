cmake_minimum_required(VERSION 3.10)
project(bikeshare)

find_package(Boost REQUIRED)

add_executable(bikeshare main.cpp include/bs/spec.hpp include/bs/graph.hpp)
target_compile_features(bikeshare PUBLIC cxx_std_17)
target_include_directories(bikeshare PUBLIC ${Boost_INCLUDE_DIRS})
target_include_directories(bikeshare PUBLIC "include")
