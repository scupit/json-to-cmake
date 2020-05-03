#ifndef FILEWRITEHELPER_HPP
#define FILEWRITEHELPER_HPP

#define PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}"
#define CMAKE_BINARY_DIR "${CMAKE_BINARY_DIR}"
#define CMAKE_BUILD_CONFIG "${CMAKE_BUILD_TYPE}"

#include <iostream>
#include <string>

std::string inBraces(const std::string&);
std::string inQuotes(const std::string&);
std::string mangleLibName(const std::string&, const std::string&);

std::string includeDirsVariable(const std::string&);
std::string headersVariable(const std::string&);
std::string sourcesVariable(const std::string&);

std::string projectSourceDir();
std::string cmakeBinaryDir();
std::string cmakeBuildType();
std::string emptyString();

std::string getOutputDir(const std::string&);

#endif