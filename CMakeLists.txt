cmake_minimum_required(VERSION 3.27)

project(cerealizer VERSION 0.0 LANGUAGES CXX)

add_library(cerealizer 
    include/cerealizer.h
    src/cerealizer.cpp)

target_include_directories(cerealizer PUBLIC include)
