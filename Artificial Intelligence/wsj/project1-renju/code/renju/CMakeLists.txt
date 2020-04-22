cmake_minimum_required(VERSION 3.15)
project(renju)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O2 -Wall")

add_executable(renju BlankEvaluate.cpp BlankEvaluate.h
 createmoves.cpp createmoves.h define.h define.cpp evaluate.cpp evaluate.h
  GameLoop.h GameLoop.cpp gameover.cpp makemove.cpp makemove.h
  printchessboard.cpp printchessboard.h searchmove.cpp searchmove.h start.cpp
  test.cpp test.h)