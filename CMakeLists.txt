cmake_minimum_required(VERSION 2.8)
project(cpp-training)
#####
if(CMAKE_COMPILER_IS_GNUCXX)
  include(CheckCXXCompilerFlag)
  CHECK_CXX_COMPILER_FLAG("-std=c++0x" has_cxx0x)
  if(has_cxx0x)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
  endif()
endif()

include_directories(.)
#####
set(training_srcs training.cpp scope_timer.hpp)
source_group("" FILES ${training_srcs})
add_executable(training ${training_srcs})
