#include "helpers/FileHelper.hpp"

#include <algorithm>
#include "helpers/StringHelper.hpp"

std::string FileHelper::projectRootString = std::filesystem::current_path().string();

std::string FileHelper::asPosix(const std::filesystem::path& thePath) {
  return asPosix(thePath.string());
}

std::string FileHelper::asPosix(std::string pathString) {
  return StringHelper::replaceAll(pathString, "\\", "/");
}

std::string FileHelper::joinPath(const std::vector<std::string>& pathSections) {
  return StringHelper::join(pathSections, "/");
}

std::string FileHelper::normalizeAbsolutePath(std::string pathToModify) {
  const size_t rootIndex = pathToModify.find(projectRootString);

  if (rootIndex != std::string::npos) {
    pathToModify.erase(rootIndex, projectRootString.size());

    // Remove leading slashes
    auto iter = pathToModify.begin();
    while(*iter == '/') {
      ++iter;
    }
    pathToModify.erase(pathToModify.begin(), iter);
  }
  return pathToModify;
}

std::filesystem::path FileHelper::projectRoot(void) {
  return std::filesystem::path(projectRootString);
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

void FileHelper::forFileInDir(const std::string& relativePath, const std::function<void(const std::filesystem::path&)>& callback) {
  getItemsThen<std::filesystem::directory_iterator>(relativePath, callback, isFile);
}

void FileHelper::forFileInDirRecursive(const std::string& relativePath, const std::function<void(const std::filesystem::path&)>& callback) {
  getItemsThen<std::filesystem::recursive_directory_iterator>(relativePath, callback, isFile);
}

void FileHelper::forDirRecursive(const std::string& relativePath, const std::function<void(const std::filesystem::path&)>& callback) {
  getItemsThen<std::filesystem::recursive_directory_iterator>(relativePath, callback, isDir);
}

void FileHelper::getFilesByExtension(std::set<std::string>& fileList, const std::string& relativePath, const std::vector<std::string>& extensionsMatching, const bool shouldSearchRecursively) {
  auto callback = [&fileList, &extensionsMatching](const std::filesystem::path& filePath) {
    if (filePath.has_extension() && std::find(extensionsMatching.begin(), extensionsMatching.end(), filePath.extension()) != extensionsMatching.end()) {
      fileList.insert(normalizeAbsolutePath(filePath.string()));
    }
  };

  if (shouldSearchRecursively) {
    forFileInDirRecursive(relativePath, callback); 
  }
  else {
    forFileInDir(relativePath, callback);
  }
}

// HELPERS

void FileHelper::createRelativeToRoot(const std::string& relativePathString) {
  std::vector<std::string> sectionsToCreate = splitPath(projectRoot().string());

  for (std::string& str : splitPath(relativePathString)) {
    sectionsToCreate.push_back(str);
  }

  std::filesystem::create_directories(std::filesystem::path(joinPath(sectionsToCreate)));
}

bool FileHelper::isFile(const std::filesystem::directory_entry& entry) {
  return entry.is_regular_file();
}

bool FileHelper::isFile(const std::string& fileName) {
  return std::filesystem::is_regular_file(std::filesystem::path(fileName));
}

bool FileHelper::isDir(const std::filesystem::directory_entry& entry) {
  return entry.is_directory();
}

bool FileHelper::isDir(const std::string& dirName) {
  return std::filesystem::is_directory(std::filesystem::path(dirName));
}