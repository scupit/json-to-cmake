#include "ImportedLib.hpp"

#include "constants/Tags.hpp"
#include "helpers/FileHelper.hpp"
#include "FileRetriever.hpp"
#include "Logger.hpp"

ImportedLib::ImportedLib(const std::string& name, JsonValue& importedLibData)
  : m_name(name),
    m_headers(std::set<std::string>()),
    m_includeDirs(std::set<std::string>()),
    m_generatedDepDirname(std::string()),
    m_gitRepoToClone(std::string()),
    m_shouldCloneRepo(true),
    m_dirContainingLibraryFiles(std::string()),
    m_libraryFileNames(std::vector<std::string>())
{
  loadRootDir(importedLibData);
  loadLibraryFiles(importedLibData);
  loadGitRepoToClone(importedLibData);
  loadGeneratedDirname(importedLibData);

  FileRetriever::loadHeaderFiles(m_headers, importedLibData);
  FileRetriever::loadIncludeDirs(m_includeDirs, importedLibData);
}

ImportedLib::~ImportedLib() { }

void ImportedLib::loadRootDir(JsonValue& importedLibData) {
  if (importedLibData.hasOwnProperty(Tags::IMPORT_ROOT_DIR)) {
    m_dirContainingLibraryFiles = FileHelper::normalizeAbsolutePath(FileHelper::resolveFromRoot(importedLibData[Tags::IMPORT_ROOT_DIR].asString()));
  }
  else {
    logErrorThenQuit(std::string("Must specify ") + Tags::IMPORT_ROOT_DIR + " in imported lib " + m_name);
  }
}

void ImportedLib::loadLibraryFiles(JsonValue& importedLibData) {
  if (importedLibData.hasOwnProperty(Tags::IMPORTED_LIB_FILES)) {
    if (importedLibData[Tags::IMPORTED_LIB_FILES].asVector().empty()) {
      logErrorThenQuit(std::string("Must specify at least one ") + Tags::IMPORTED_LIB_FILES + " in Imported Lib " + m_name);
    }
    else {
      for (JsonValue& libFileName : importedLibData[Tags::IMPORTED_LIB_FILES].asVector()) {
        m_libraryFileNames.push_back(libFileName.asString());
      }
    }
  }
  else {
    logErrorThenQuit(std::string(Tags::IMPORTED_LIB_FILES) + " are required (but not yet defined) in imported lib " + m_name);
  }
}

void ImportedLib::loadGitRepoToClone(JsonValue& importedLibData) {
  if (importedLibData.hasOwnProperty(Tags::IMPORT_GIT_REPO)) {
    m_gitRepoToClone = importedLibData[Tags::IMPORT_GIT_REPO].asString();

    if (importedLibData.hasOwnProperty(Tags::IMPORT_SHOULD_CLONE_REPO)) {
      m_shouldCloneRepo = importedLibData[Tags::IMPORT_SHOULD_CLONE_REPO].asBool();
    }
  }
}

void ImportedLib::loadGeneratedDirname(JsonValue& importedLibData) {
  if (importedLibData.hasOwnProperty(Tags::IMPORT_GENERATED_DIRNAME)) {
    m_generatedDepDirname = importedLibData[Tags::IMPORT_GENERATED_DIRNAME].asString();
  }
  else {
    m_generatedDepDirname = m_name;
  }
}