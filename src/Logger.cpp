#include "Logger.hpp"

void logErrorThenQuit(const std::string& issue) {
  std::cerr << issue << std::endl;
  exit(EXIT_SUCCESS);
}