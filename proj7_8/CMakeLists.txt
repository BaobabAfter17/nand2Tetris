cmake_minimum_required(VERSION 3.0.0)
project(VMTranslator)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(VMTranslator
  src/main.cpp
  src/parser.cpp
  src/code_writer.cpp)
