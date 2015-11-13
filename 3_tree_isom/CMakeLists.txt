cmake_minimum_required (VERSION 2.6)


project (tree_isom)

set ( EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin )
set ( CMAKE_CXX_FLAGS "-g -std=c++11" )
set ( DEBUG_MODE ON )

file ( GLOB MY_HEADERS src/*.h )
file ( GLOB MY_SOURCES src/*.cpp )

add_executable ( tree_isom
	${MY_HEADERS}
	${MY_SOURCES}
)

