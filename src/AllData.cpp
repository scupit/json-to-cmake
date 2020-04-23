#include "AllData.hpp"

#include <stdexcept>
#include "JSON/JsonParser.hpp"
#include "JSON/JsonValue.hpp"
#include "helpers/FileHelper.hpp"
#include "constants/Globals.hpp"
#include "constants/Tags.hpp"
#include "Logger.hpp"
#include <iostream>

AllData::AllData(const char* projectRoot)
  : m_outputs(std::vector<OutputItem>()),
    m_outputGroups(std::vector<OutputGroup>())
{

  const std::string jsonFilePath = FileHelper::resolve(std::string(projectRoot), Globals::JSON_FILE_NAME);

  try {
    JsonValue jsonData = JsonParser(jsonFilePath).getJsonCopy();
    loadOutputs(jsonData);
    loadGroups(jsonData);
    loadImportedLibs(jsonData);
  }
  catch (std::runtime_error& e) {
    logErrorThenQuit(jsonFilePath + " does not exist.");
  }
}

AllData::~AllData() { }

void AllData::loadOutputs(JsonValue& jsonData) {
  if (jsonData.hasOwnProperty(Tags::OUTPUT)) {
    for (auto& [name, outputItemData] : jsonData[Tags::OUTPUT].asMap()) {
      m_outputs.emplace_back(name, outputItemData);
    }
  }
}

void AllData::loadGroups(JsonValue& jsonData) {
  if (jsonData.hasOwnProperty(Tags::OUTPUT_GROUPS)) {
    for (auto& [name, outputGroupData] : jsonData[Tags::OUTPUT_GROUPS].asMap()) {
      m_outputGroups.emplace_back(name, outputGroupData);
    }
  }
}

void AllData::loadImportedLibs(JsonValue& jsonData) {
  if (jsonData.hasOwnProperty(Tags::IMPORTED_LIBRARIES)) {
    for (auto& [name, importedLibData] : jsonData[Tags::IMPORTED_LIBRARIES].asMap()) {
      ImportedLib lib(name, importedLibData);
      m_importedLibs.push_back(lib);
      // TODO: Clone the associated git repo
      // if (lib.shouldCloneRepo() && lib.hasRepoToClone()) {

      // }
    }
  }
  // TODO: Generate dep directories
}