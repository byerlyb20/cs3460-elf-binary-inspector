cmake_minimum_required(VERSION 3.12) 

project(ELFBinaryInspector) 

add_executable(ELFBinaryInspector src/main.cpp)

target_sources(ELFBinaryInspector PRIVATE
    src/binary_reader.cpp
    src/file_buffer.cpp
    src/read_validate_elf64.cpp
)

set_property(TARGET ELFBinaryInspector PROPERTY CXX_STANDARD 20)