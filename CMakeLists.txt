set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_VERBOSE_MAKEFILE ON)

project(qbe)
cmake_minimum_required( VERSION 2.8.8 )

set( CMAKE_MODULE_PATH ~/cmake )

set( GCC_COVERAGE_COMPILE_FLAGS "-std=c++11" )
# set( GCC_COVERAGE_LINK_FLAGS "-lSDL2 -lGL" )

SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS} -Wall" )
# SET( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}" )
# Instead of using these flags, we started using 'target_link_libraries
# located under 'src/CMakeLists.txt'

include_directories( 
  "./include" )

SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR} )

add_subdirectory( "src" )
