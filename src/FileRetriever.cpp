#include "FileRetriever.hpp"

#include "helpers/FileHelper.hpp"
#include "constants/Tags.hpp"

void FileRetriever::loadSourceFiles(std::set<std::string>& sourcesSet, JsonValue& jsonDataItem) {
  sourcesSet.clear();

  const std::vector<std::string> sourceExtensions = {
    ".c",
    ".cpp",
    ".cxx",
    ".c++"
  };

  if (jsonDataItem.hasOwnProperty(Tags::R_SOURCE_DIRS)) {
    for (JsonValue& givenDir : jsonDataItem[Tags::R_SOURCE_DIRS].asVector()) {
      FileHelper::getFilesByExtension(sourcesSet, givenDir.asString(), sourceExtensions, true);
    }
  }

  if (jsonDataItem.hasOwnProperty(Tags::SOURCE_DIRS)) {
    for (JsonValue& givenDir : jsonDataItem[Tags::SOURCE_DIRS].asVector()) {
      FileHelper::getFilesByExtension(sourcesSet, givenDir.asString(), sourceExtensions, false);
    }
  }

  if (jsonDataItem.hasOwnProperty(Tags::INDIVIDUAL_SOURCES)) {
    for (JsonValue& givenDir : jsonDataItem[Tags::INDIVIDUAL_SOURCES].asVector()) {
      sourcesSet.insert(FileHelper::resolveFromRoot(givenDir.asString()));
    }
  }
}

void FileRetriever::loadHeaderFiles(std::set<std::string>& headerSet, JsonValue& jsonDataItem) {
  headerSet.clear();

  const std::vector<std::string> headerExtensions = {
    ".h",
    ".hpp",
    ".h++",
    ".hxx",
    ".tcc",
    ".tpp",
    ".txx",
    ".t++"
  };

  if (jsonDataItem.hasOwnProperty(Tags::R_HEADER_DIRS)) {
    for (JsonValue& givenDir : jsonDataItem[Tags::R_HEADER_DIRS].asVector()) {
      FileHelper::getFilesByExtension(headerSet, givenDir.asString(), headerExtensions, true);
    }
  }

  if (jsonDataItem.hasOwnProperty(Tags::HEADER_DIRS)) {
    for (JsonValue& givenDir : jsonDataItem[Tags::HEADER_DIRS].asVector()) {
      FileHelper::getFilesByExtension(headerSet, givenDir.asString(), headerExtensions, false);
    }
  }

  if (jsonDataItem.hasOwnProperty(Tags::INDIVIDUAL_HEADERS)) {
    for (JsonValue& givenDir : jsonDataItem[Tags::INDIVIDUAL_HEADERS].asVector()) {
      headerSet.insert(FileHelper::resolveFromRoot(givenDir.asString()));
    }
  }
}

void FileRetriever::loadIncludeDirs(std::set<std::string>& includeDirSet, JsonValue& jsonDataItem) {
  includeDirSet.clear();

  if (jsonDataItem.hasOwnProperty(Tags::R_INCLUDE_DIRS)) {
    for (JsonValue& givenDir : jsonDataItem[Tags::R_INCLUDE_DIRS].asVector()) {
      FileHelper::forDirRecursive(givenDir.asString(), [&includeDirSet](const std::filesystem::path& filePath) {
        includeDirSet.insert(FileHelper::normalizeAbsolutePath(filePath.string()));
      });
    }
  }

  if (jsonDataItem.hasOwnProperty(Tags::INCLUDE_DIRS)) {
    for (JsonValue& givenDir : jsonDataItem[Tags::INCLUDE_DIRS].asVector()) {
      includeDirSet.insert(FileHelper::normalizeAbsolutePath(FileHelper::resolveFromRoot(givenDir.asString())));
    }
  }
}