cmake_minimum_required(VERSION 2.8.11)
option(test "Include Tests." OFF)

set(PROJECT_NAME ttt)
project(${PROJECT_NAME} C CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
else()
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support.")
    message(STATUS "Please use a different C++ compiler.")
endif()

#dep

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
find_package(Threads REQUIRED)

set(INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/include)
set(LIBRARIES ${CMAKE_THREAD_LIBS_INIT})


# src

add_subdirectory(src/)

# global vars

SET(${PROJECT_NAME}_INCLUDE_DIRS ${INCLUDE_DIRS}
  CACHE INTERNAL
  "${PROJECT_NAME} Include Directories"
  FORCE
)

SET(${PROJECT_NAME}_LIBRARIES ${LIBRARIES}
  CACHE INTERNAL
  "${PROJECT_NAME} Link Libraries"
  FORCE
)

# test

if (test)
  add_subdirectory(lib/googletest)
  enable_testing()
  include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

  add_subdirectory(tests/)
endif(test)

