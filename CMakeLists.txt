cmake_minimum_required(VERSION 3.12) 

project(ELFBinaryInspector) 

add_executable(ELFBinaryInspector src/main.cpp)

set_property(TARGET ELFBinaryInspector PROPERTY CXX_STANDARD 20)