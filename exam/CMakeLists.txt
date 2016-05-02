cmake_minimum_required ( VERSION 2.6 )

SET ( VERBOSE_MAKEFILE on )
SET ( CMAKE_CXX_COMPILER "g++-5.2.0" )
SET ( CMAKE_C_COMPILER "gcc-5.2.0" )


project ( exam )

set ( EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin )
set ( CMAKE_CXX_FLAGS "-g -std=c++11" )
#set ( CMAKE_CXX_FLAGS "-std=c++11 -O3" )
set ( DEBUG_MODE ON )

#ADD_DEFINITIONS ( -DMEASURE_TIME )

file ( GLOB MY_HEADERS src/*.h )
file ( GLOB MY_SOURCES src/*.cpp )


add_executable ( exam
    ${MY_HEADERS}
    ${MY_SOURCES}
)

