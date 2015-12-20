cmake_minimum_required (VERSION 2.6)

SET ( CMAKE_CXX_COMPILER "g++-5.2.0" )
SET ( CMAKE_C_COMPILER "gcc-5.2.0" )


project (automaton)

set ( EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin )
#set ( CMAKE_CXX_FLAGS "-g -std=c++11" )
set ( CMAKE_CXX_FLAGS "-std=c++11 -O3" )
#set ( DEBUG_MODE ON )

ADD_DEFINITIONS ( -DMEASURE_TIME )
#ADD_DEFINITIONS ( -DINCLUDE_NEGATIVES )
#ADD_DEFINITIONS ( -DDISABLE_RAW_POINTERS )

file ( GLOB MY_HEADERS src/*.h )
file ( GLOB MY_SOURCES src/*.cpp )


add_executable ( automaton
    src/IncompleteAutomaton.h
    src/InitialFinalStates.h
    src/IncompleteAutomaton.cpp
    src/InitialFinalStates.cpp
    src/main.cpp
)

add_executable ( automaton_optimized
    src/main_optimized.cpp
)

