cmake_minimum_required (VERSION 2.6)


project (backup_con)

set ( EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin )
set ( CMAKE_CXX_FLAGS "-g" )
set ( DEBUG_MODE ON )

file ( GLOB MY_HEADERS src/*.h )
file ( GLOB MY_SOURCES src/*.cpp )

add_executable ( backup_con
	${MY_HEADERS}
	${MY_SOURCES}
)

