################################################################################
# Generated with Skylar Cupit's json-to-cmake tool
# Github: https://github.com/scupit/json-to-cmake
################################################################################

cmake_minimum_required( VERSION 3.12 )
project(  LANGUAGES C CXX )

# ////////////////////////////////////////////////////////////////////////////////
# LANGUAGE STANDARDS
# ////////////////////////////////////////////////////////////////////////////////

set( CMAKE_C_STANDARD 99 CACHE STRING "C compiler standard" )
set_property( CACHE CMAKE_C_STANDARD PROPERTY STRINGS 99 )
message( "Using C compiler standard --std=c${CMAKE_C_STANDARD}" )

set( CMAKE_CXX_STANDARD 17 CACHE STRING "CXX compiler standard" )
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
	set( CMAKE_BUILD_TYPE "Release" CACHE STRING "Project Configuration" FORCE )
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

# Imported library: JSON
set( JSON_INCLUDE_DIRS
	${PROJECT_SOURCE_DIR}/dep/include
)

set( JSON_HEADERS
	${PROJECT_SOURCE_DIR}/dep/include/JSON/JsonParser.hpp
	${PROJECT_SOURCE_DIR}/dep/include/JSON/JsonValue.hpp
)

find_library( JSON--jParser
	NAMES jParser
	PATHS ${PROJECT_SOURCE_DIR}/dep/lib/JSON
)

# ////////////////////////////////////////////////////////////////////////////////
# EXECUTABLE OUTPUT GROUPS
# ////////////////////////////////////////////////////////////////////////////////

# Output Group g_testing
set( g_testing_HEADERS
	${JSON_HEADERS}
	${PROJECT_SOURCE_DIR}/include/AllData.hpp
	${PROJECT_SOURCE_DIR}/include/FileRetriever.hpp
	${PROJECT_SOURCE_DIR}/include/ImportedLib.hpp
	${PROJECT_SOURCE_DIR}/include/Linker.hpp
	${PROJECT_SOURCE_DIR}/include/Logger.hpp
	${PROJECT_SOURCE_DIR}/include/constants/Globals.hpp
	${PROJECT_SOURCE_DIR}/include/constants/Tags.hpp
	${PROJECT_SOURCE_DIR}/include/file-writing/CmakeCustomFunctions.hpp
	${PROJECT_SOURCE_DIR}/include/file-writing/FileWriter.hpp
	${PROJECT_SOURCE_DIR}/include/general-data/BuildTarget.hpp
	${PROJECT_SOURCE_DIR}/include/general-data/GeneralData.hpp
	${PROJECT_SOURCE_DIR}/include/helpers/FileHelper.hpp
	${PROJECT_SOURCE_DIR}/include/helpers/FileWriteHelper.hpp
	${PROJECT_SOURCE_DIR}/include/helpers/GitHelper.hpp
	${PROJECT_SOURCE_DIR}/include/helpers/StringHelper.hpp
	${PROJECT_SOURCE_DIR}/include/output/OutputBase.hpp
	${PROJECT_SOURCE_DIR}/include/output/OutputGroup.hpp
	${PROJECT_SOURCE_DIR}/include/output/OutputItem.hpp
)

set( g_testing_SOURCES
	${g_testing_HEADERS}
	${PROJECT_SOURCE_DIR}/src/AllData.cpp
	${PROJECT_SOURCE_DIR}/src/FileRetriever.cpp
	${PROJECT_SOURCE_DIR}/src/ImportedLib.cpp
	${PROJECT_SOURCE_DIR}/src/Linker.cpp
	${PROJECT_SOURCE_DIR}/src/Logger.cpp
	${PROJECT_SOURCE_DIR}/src/file-writing/FileWriter.cpp
	${PROJECT_SOURCE_DIR}/src/general-data/BuildTarget.cpp
	${PROJECT_SOURCE_DIR}/src/general-data/GeneralData.cpp
	${PROJECT_SOURCE_DIR}/src/helpers/FileHelper.cpp
	${PROJECT_SOURCE_DIR}/src/helpers/FileWriteHelper.cpp
	${PROJECT_SOURCE_DIR}/src/helpers/GitHelper.cpp
	${PROJECT_SOURCE_DIR}/src/helpers/StringHelper.cpp
	${PROJECT_SOURCE_DIR}/src/output/OutputBase.cpp
	${PROJECT_SOURCE_DIR}/src/output/OutputGroup.cpp
	${PROJECT_SOURCE_DIR}/src/output/OutputItem.cpp
)

set( g_testing_INCLUDE_DIRS
	${JSON_INCLUDE_DIRS}
	${PROJECT_SOURCE_DIR}/include
)

# Output executable: visualize-links
set( visualize-links_HEADERS
	${g_testing_HEADERS}
)

set( visualize-links_SOURCES
	${visualize-links_HEADERS}
	${g_testing_SOURCES}
	${PROJECT_SOURCE_DIR}/tests/visualize-link-tree-main.cpp
)

set( visualize-links_INCLUDE_DIRS
	${g_testing_INCLUDE_DIRS}
)

add_executable( visualize-links ${visualize-links_SOURCES} )
target_include_directories( visualize-links PRIVATE ${visualize-links_INCLUDE_DIRS} )

set_target_properties( visualize-links PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}
)

# ////////////////////////////////////////////////////////////////////////////////
# INDIVIDUAL OUTPUT EXECUTABLES
# ////////////////////////////////////////////////////////////////////////////////

# Output executable: gcmake
set( gcmake_HEADERS
	${JSON_HEADERS}
	${PROJECT_SOURCE_DIR}/include/AllData.hpp
	${PROJECT_SOURCE_DIR}/include/FileRetriever.hpp
	${PROJECT_SOURCE_DIR}/include/ImportedLib.hpp
	${PROJECT_SOURCE_DIR}/include/Linker.hpp
	${PROJECT_SOURCE_DIR}/include/Logger.hpp
	${PROJECT_SOURCE_DIR}/include/constants/Globals.hpp
	${PROJECT_SOURCE_DIR}/include/constants/Tags.hpp
	${PROJECT_SOURCE_DIR}/include/file-writing/CmakeCustomFunctions.hpp
	${PROJECT_SOURCE_DIR}/include/file-writing/FileWriter.hpp
	${PROJECT_SOURCE_DIR}/include/general-data/BuildTarget.hpp
	${PROJECT_SOURCE_DIR}/include/general-data/GeneralData.hpp
	${PROJECT_SOURCE_DIR}/include/helpers/FileHelper.hpp
	${PROJECT_SOURCE_DIR}/include/helpers/FileWriteHelper.hpp
	${PROJECT_SOURCE_DIR}/include/helpers/GitHelper.hpp
	${PROJECT_SOURCE_DIR}/include/helpers/StringHelper.hpp
	${PROJECT_SOURCE_DIR}/include/output/OutputBase.hpp
	${PROJECT_SOURCE_DIR}/include/output/OutputGroup.hpp
	${PROJECT_SOURCE_DIR}/include/output/OutputItem.hpp
)

set( gcmake_SOURCES
	${gcmake_HEADERS}
	${PROJECT_SOURCE_DIR}/main.cpp
	${PROJECT_SOURCE_DIR}/src/AllData.cpp
	${PROJECT_SOURCE_DIR}/src/FileRetriever.cpp
	${PROJECT_SOURCE_DIR}/src/ImportedLib.cpp
	${PROJECT_SOURCE_DIR}/src/Linker.cpp
	${PROJECT_SOURCE_DIR}/src/Logger.cpp
	${PROJECT_SOURCE_DIR}/src/file-writing/FileWriter.cpp
	${PROJECT_SOURCE_DIR}/src/general-data/BuildTarget.cpp
	${PROJECT_SOURCE_DIR}/src/general-data/GeneralData.cpp
	${PROJECT_SOURCE_DIR}/src/helpers/FileHelper.cpp
	${PROJECT_SOURCE_DIR}/src/helpers/FileWriteHelper.cpp
	${PROJECT_SOURCE_DIR}/src/helpers/GitHelper.cpp
	${PROJECT_SOURCE_DIR}/src/helpers/StringHelper.cpp
	${PROJECT_SOURCE_DIR}/src/output/OutputBase.cpp
	${PROJECT_SOURCE_DIR}/src/output/OutputGroup.cpp
	${PROJECT_SOURCE_DIR}/src/output/OutputItem.cpp
)

set( gcmake_INCLUDE_DIRS
	${JSON_INCLUDE_DIRS}
	${PROJECT_SOURCE_DIR}/include
)

add_executable( gcmake ${gcmake_SOURCES} )
target_include_directories( gcmake PRIVATE ${gcmake_INCLUDE_DIRS} )

set_target_properties( gcmake PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}
)

# ////////////////////////////////////////////////////////////////////////////////
# LINK LIBRARIES TO OUTPUTS
# ////////////////////////////////////////////////////////////////////////////////

target_link_libraries( visualize-links
	${JSON--jParser}
)

target_link_libraries( gcmake
	${JSON--jParser}
)

# ////////////////////////////////////////////////////////////////////////////////
# IMPORTED LIB COPY COMMANDS
# ////////////////////////////////////////////////////////////////////////////////

# Copy libraries imported by JSON to executable output dir
add_custom_command(TARGET gcmake POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
		${PROJECT_SOURCE_DIR}/dep/lib/JSON
		${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}
)

# Copy libraries imported by JSON to executable output dir
add_custom_command(TARGET visualize-links POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
		${PROJECT_SOURCE_DIR}/dep/lib/JSON
		${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}
)

