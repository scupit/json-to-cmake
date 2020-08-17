#include "general-data/BuildTarget.hpp"
#include "constants/Tags.hpp"

#include "helpers/FileHelper.hpp"

BuildTarget::BuildTarget(const std::string& name, JsonValue& buildTargetData)
  : m_name(name),
    m_compilerFlags(std::vector<std::string>()),
    m_compilerDefinitions(std::vector<std::string>()),
    m_otherFiles(std::vector<std::string>()),
    m_otherDirs(std::vector<std::string>())
{
  loadCompilerFlags(buildTargetData);
  loadCompilerDefinitions(buildTargetData);
  loadOtherFiles(buildTargetData);
  loadOtherDirs(buildTargetData);
}

BuildTarget::~BuildTarget() { }

void BuildTarget::loadCompilerFlags(JsonValue& buildTargetData) {
  if (buildTargetData.hasOwnProperty(Tags::COMPILER_FLAGS)) {
    for (JsonValue& flag : buildTargetData[Tags::COMPILER_FLAGS].asVector()) {
      m_compilerFlags.push_back(flag.asString());
    }
  }
}

void BuildTarget::loadCompilerDefinitions(JsonValue& buildTargetData) {
  if (buildTargetData.hasOwnProperty(Tags::COMPILER_DEFINES)) {
    for (JsonValue& definition : buildTargetData[Tags::COMPILER_DEFINES].asVector()) {
      m_compilerDefinitions.push_back(definition.asString()); 
    }
  }
}

void BuildTarget::loadOtherFiles(JsonValue& buildTargetData) {
  if (buildTargetData.hasOwnProperty(Tags::OTHER_FILES)) {
    for (JsonValue& fileName : buildTargetData[Tags::OTHER_FILES].asVector()) {
      m_otherFiles.push_back(FileHelper::normalizeAbsolutePath(FileHelper::resolveFromRoot(fileName.asString())));
    }
  }
}

void BuildTarget::loadOtherDirs(JsonValue& buildTargetData) {
  if (buildTargetData.hasOwnProperty(Tags::OTHER_DIRS)) {
    for (JsonValue& dirName : buildTargetData[Tags::OTHER_DIRS].asVector()) {
      m_otherDirs.push_back(FileHelper::normalizeAbsolutePath(FileHelper::resolveFromRoot(dirName.asString())));
    }
  }
}