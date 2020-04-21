#include "FileHelper.hpp"
#include "StringHelper.hpp"

std::string FileHelper::asPosix(const std::filesystem::path& thePath) {
  return asPosix(thePath.string());
}

std::string FileHelper::asPosix(std::string pathString) {
  return StringHelper::replaceAll(pathString, "\\", "/");
}

std::string FileHelper::joinPath(const std::vector<std::string>& pathSections) {
  return StringHelper::join(pathSections, "/");
}

std::filesystem::path FileHelper::projectRoot(void) {
  return std::filesystem::current_path();
}

std::string FileHelper::resolve(const std::string& resolvingFrom, const std::string& relativeResolvingTo) {
  std::vector<std::string> root = splitPath(asPosix(resolvingFrom));
  const std::vector<std::string>& pieces = splitPath(asPosix(relativeResolvingTo));

  for (const std::string& piece : pieces) {
    if (piece == ".." && !root.empty()) {
      root.pop_back();
    }
    else if (piece == ".") {
      continue;
    }
    else {
      root.push_back(piece);
    }
  }

  return joinPath(root); 
}

std::string FileHelper::resolve(const std::filesystem::path& resolvingFrom, const std::string& resolvingTo) {
  return resolve(resolvingFrom.string(), resolvingTo);
}

std::string FileHelper::resolveFromRoot(const std::string& pathToResolve) {
  return resolve(projectRoot(), pathToResolve);
}

std::vector<std::string> FileHelper::splitPath(const std::string& pathToSplit) {
  return StringHelper::split(pathToSplit, "/");
}