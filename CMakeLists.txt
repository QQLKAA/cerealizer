cmake_minimum_required(VERSION 3.27)

project(cerealizer VERSION 0.0 LANGUAGES CXX)

add_library(cerealizer 
    include/cerealizer.h
    src/cerealizer.cpp)

target_include_directories(cerealizer PUBLIC include)

if ("${CMAKE_CXX_BYTE_ORDER}" STREQUAL "LITTLE_ENDIAN")
    target_compile_definitions(cerealizer PUBLIC CEREALIZER_LITTLE_ENDIAN)
elseif ("${CMAKE_CXX_BYTE_ORDER}" STREQUAL "BIG_ENDIAN")
    target_compile_definitions(cerealizer PUBLIC CEREALIZER_BIG_ENDIAN)
else()
    message(FATAL_ERROR "Cannot detect endianness")
endif()
