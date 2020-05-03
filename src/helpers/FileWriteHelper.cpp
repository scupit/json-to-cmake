#include "helpers/FileWriteHelper.hpp"

std::string inBraces(const std::string& variableName) {
  return "${" + variableName + "}";
}

std::string inQuotes(const std::string& text) {
  return '"' + text + '"';
}

std::string mangleLibName(const std::string& libName, const std::string& libFileName) {
  return libName + "--" + libFileName;
}

std::string includeDirsVariable(const std::string& variableName) {
  return variableName + "_INCLUDE_DIRS";
}

std::string headersVariable(const std::string& variableName) {
  return variableName + "_HEADERS";
}

std::string sourcesVariable(const std::string& variableName) {
  return variableName + "_SOURCES";
}

std::string getOutputDir(const std::string& dirname) {
  return std::string(CMAKE_BINARY_DIR) + "/" + dirname + "/" + CMAKE_BUILD_CONFIG;
}