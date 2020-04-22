#ifndef GCMAKE_GLOBALS_HPP
#define GCMAKE_GLOBALS_HPP

namespace Globals {
  const char* const CMAKE_VERSION = "3.12";
  const char* const CMAKE_FILE_NAME = "CMakeLists.txt";
  const char* const JSON_FILE_NAME = "cmake_data.json";

  const char* const DEFAULT_C_STANDARD = "99";
  const char* const DEFAULT_CPP_STANDARD = "11";

  const char* const DEPENDENCY_DIR = "dep";
  const char* const DEPENDENCY_LIB_DIR = "lib";
  const char* const DEPENDENCY_INCLUDE_DIR = "include";

  const char* const EXTERNAL_GIT_REPO_DIR = "external";
  const char* const EXTERNAL_BUILD_REPO_DIR = "_builds";

  const char* const OUTPUT_GROUP_NAME_PREFIX = "g_";
}

#endif