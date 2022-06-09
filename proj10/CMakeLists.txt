cmake_minimum_required(VERSION 3.0.0)
project(JackAnalyzer)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(JackAnalyzer
  src/main.cpp
  src/compilation_engine.cpp
  src/tokenizer.cpp)
