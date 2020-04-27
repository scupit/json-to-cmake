################################################################################
# Generated with Skylar Cupit's json-to-cmake tool                             #
# GitHub: https://github.com/scupit/json-to-cmake-rewrite                      #
################################################################################

cmake_minimum_required( VERSION 3.12 )
project( json-to-cmake )

# ////////////////////////////////////////////////////////////////////////////////
# LANGUAGE STANDARDS
# ////////////////////////////////////////////////////////////////////////////////

set( CMAKE_C_STANDARD 99 CACHE STRING "C compiler standard year" )
set_property( CACHE CMAKE_C_STANDARD PROPERTY STRINGS 99 )
message( "Using C compiler standard --std=c${CMAKE_C_STANDARD}" )

set( CMAKE_CXX_STANDARD 17 CACHE STRING "CXX compiler standard year" )
set_property( CACHE CMAKE_CXX_STANDARD PROPERTY STRINGS 17 )
message( "Using CXX compiler standard --std=c++${CMAKE_CXX_STANDARD}" )

set( CMAKE_C_STANDARD_REQUIRED ON )
set( CMAKE_CXX_STANDARD_REQUIRED ON )

set( CMAKE_C_EXTENSIONS OFF )
set( CMAKE_CXX_EXTENSIONS OFF )

# ////////////////////////////////////////////////////////////////////////////////
# BUILD TARGETS
# ////////////////////////////////////////////////////////////////////////////////

set_property( CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" )

if( "${CMAKE_BUILD_TYPE}" STREQUAL "" )
	set( CMAKE_BUILD_TYPE "Debug" CACHE STRING "Project Configuration" FORCE )
endif()

if( "${CMAKE_BUILD_TYPE}" STREQUAL "Debug" )
	set( CMAKE_CXX_FLAGS "-g -Wall -Wextra -Wconversion -Wuninitialized -pedantic -pedantic-errors " )
	set( CMAKE_C_FLAGS "-g -Wall -Wextra -Wconversion -Wuninitialized -pedantic -pedantic-errors " )
elseif( "${CMAKE_BUILD_TYPE}" STREQUAL "Release" )
	set( CMAKE_CXX_FLAGS "-O2 -s " )
	set( CMAKE_C_FLAGS "-O2 -s " )
	add_compile_definitions( NDEBUG )
endif()

message( "Using compiler flags: ${CMAKE_CXX_FLAGS}" )
message( "Building project '${CMAKE_BUILD_TYPE}' configuration" )

# ////////////////////////////////////////////////////////////////////////////////
# IMPORTED LIBRARIES 
# ////////////////////////////////////////////////////////////////////////////////

# Imported lib: JSON
set( JSON_INCLUDE_DIRS
	${PROJECT_SOURCE_DIR}/dep/include
)

set( JSON_HEADERS
	${PROJECT_SOURCE_DIR}/dep/include/JSON/JsonParser.hpp
	${PROJECT_SOURCE_DIR}/dep/include/JSON/JsonValue.hpp
)

find_library( JSON_0
	NAMES jParser
	PATHS ${PROJECT_SOURCE_DIR}/dep/lib/JSON
)

# ////////////////////////////////////////////////////////////////////////////////
# OUTPUT EXECUTABLES 
# ////////////////////////////////////////////////////////////////////////////////

# Output executable: gen-cmake
set( gen-cmake_HEADERS
	${JSON_HEADERS}
	${PROJECT_SOURCE_DIR}/include/AllData.hpp
	${PROJECT_SOURCE_DIR}/include/FileRetriever.hpp
	${PROJECT_SOURCE_DIR}/include/ImportedLib.hpp
	${PROJECT_SOURCE_DIR}/include/Logger.hpp
	${PROJECT_SOURCE_DIR}/include/constants/Globals.hpp
	${PROJECT_SOURCE_DIR}/include/constants/Tags.hpp
	${PROJECT_SOURCE_DIR}/include/helpers/FileHelper.hpp
	${PROJECT_SOURCE_DIR}/include/helpers/GitHelper.hpp
	${PROJECT_SOURCE_DIR}/include/helpers/StringHelper.hpp
	${PROJECT_SOURCE_DIR}/include/output/OutputBase.hpp
	${PROJECT_SOURCE_DIR}/include/output/OutputGroup.hpp
	${PROJECT_SOURCE_DIR}/include/output/OutputItem.hpp
)

set( gen-cmake_SOURCES
	${gen-cmake_HEADERS}
	${PROJECT_SOURCE_DIR}/main.cpp
	${PROJECT_SOURCE_DIR}/src/AllData.cpp
	${PROJECT_SOURCE_DIR}/src/FileRetriever.cpp
	${PROJECT_SOURCE_DIR}/src/ImportedLib.cpp
	${PROJECT_SOURCE_DIR}/src/Logger.cpp
	${PROJECT_SOURCE_DIR}/src/helpers/FileHelper.cpp
	${PROJECT_SOURCE_DIR}/src/helpers/GitHelper.cpp
	${PROJECT_SOURCE_DIR}/src/helpers/StringHelper.cpp
	${PROJECT_SOURCE_DIR}/src/output/OutputBase.cpp
	${PROJECT_SOURCE_DIR}/src/output/OutputGroup.cpp
	${PROJECT_SOURCE_DIR}/src/output/OutputItem.cpp
)

set( gen-cmake_INCLUDE_DIRS
	${JSON_INCLUDE_DIRS}
	${PROJECT_SOURCE_DIR}/include
)

add_executable( gen-cmake ${gen-cmake_SOURCES} )
target_include_directories( gen-cmake PRIVATE ${gen-cmake_INCLUDE_DIRS} )

set_target_properties( gen-cmake PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}
)

# ////////////////////////////////////////////////////////////////////////////////
# LINK LIBRARIES TO OUTPUTS
# ////////////////////////////////////////////////////////////////////////////////

target_link_libraries( gen-cmake
	${JSON_0}
)

# ////////////////////////////////////////////////////////////////////////////////
# IMPORTED LIBRARY COPY COMMANDS
# ////////////////////////////////////////////////////////////////////////////////

# Copy libaries imported by JSON to executable output dir
add_custom_command(TARGET gen-cmake POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
		${PROJECT_SOURCE_DIR}/dep/lib/JSON
		${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}
)