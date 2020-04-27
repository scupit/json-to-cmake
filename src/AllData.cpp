#include "AllData.hpp"

#include <functional>
#include <stdexcept>
#include <unordered_set>
#include "JSON/JsonParser.hpp"
#include "JSON/JsonValue.hpp"
#include "helpers/FileHelper.hpp"
#include "helpers/GitHelper.hpp"
#include "constants/Globals.hpp"
#include "constants/Tags.hpp"
#include "Logger.hpp"
#include <iostream>

AllData::AllData()
  : m_outputs(std::vector<OutputItem>()),
    m_outputGroups(std::vector<OutputGroup>())
{

  const std::string jsonFilePath = FileHelper::resolveFromRoot(Globals::JSON_FILE_NAME);

  try {
    JsonValue jsonData = JsonParser(jsonFilePath).getJsonCopy();
    loadOutputs(jsonData);
    loadGroups(jsonData);
    loadImportedLibs(jsonData);
    validateLoadedItems();
  }
  catch (std::runtime_error& e) {
    logErrorThenQuit(e.what());
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

      FileHelper::createRelativeToRoot(FileHelper::joinPath({
        Globals::DEPENDENCY_DIR,
        Globals::DEPENDENCY_LIB_DIR,
        lib.generatedDirname()
      }));

      FileHelper::createRelativeToRoot(FileHelper::joinPath({
        Globals::DEPENDENCY_DIR,
        Globals::DEPENDENCY_INCLUDE_DIR,
        lib.generatedDirname()
      }));

      if (lib.shouldCloneRepo() && lib.hasRepoToClone()) {
        cloneRepoIfNonexistent(lib.gitRepoToClone(), lib.generatedDirname());
      }
    }
  }
}

void AllData::validateLoadedItems() {
  if (!anyOutputsDefined()) {
    throw std::runtime_error(std::string("Must define at least one ") + Tags::OUTPUT + " or " + Tags::OUTPUT_GROUPS);
  }

  std::unordered_set<std::string> itemNames;

  for (const OutputItem& output : m_outputs) {
    itemNames.insert(output.name());
  }

  auto checkName = [&itemNames](auto& itemToCheck, const std::string& errorMessage) -> void {
    if (itemNames.find(itemToCheck.name()) == itemNames.end()) {
      itemNames.insert(itemToCheck.name());
    }
    else {
      throw std::runtime_error(errorMessage);
    }
  };

  for (OutputGroup& group : m_outputGroups) {
    checkName(group, "Group name " + group.name() + " collides with an output name.");

    for (OutputItem& output : group.outputs()) {
      checkName(output, "Output \"" + output.name() + "\" in group " + group.name() + " has a colliding name.");
    }
  }

  for (const ImportedLib& lib : m_importedLibs) {
    checkName(lib, "Imported library " + lib.name() + "has a colliding name.");
  }
}

bool AllData::anyOutputsDefined() {
  return !(m_outputs.empty() && m_outputGroups.empty());
}