#include "AllData.hpp"

#include <stdexcept>
#include "JSON/JsonParser.hpp"
#include "helpers/FileHelper.hpp"
#include "constants/Globals.hpp"
#include "constants/Tags.hpp"
#include "Logger.hpp"

AllData::AllData()
  : m_outputs(std::vector<OutputItem>()),
    m_outputGroups(std::vector<OutputGroup>())
{
  try {
    JsonParser jsonData(FileHelper::resolveFromRoot(Globals::JSON_FILE_NAME));
    loadOutputs(jsonData.getJsonReference());
    loadGroups(jsonData.getJsonReference());
  }
  catch (std::runtime_error& e) {
    logErrorThenQuit(FileHelper::resolveFromRoot(Globals::JSON_FILE_NAME) + " does not exist.");
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