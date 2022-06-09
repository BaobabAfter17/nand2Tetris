cmake_minimum_required(VERSION 3.0.0)
project(Assembler)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(Assembler
  src/main.cpp
  src/remove_comment.cpp
  src/parse_symbols.cpp
  src/assemble.cpp)
