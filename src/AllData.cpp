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
#include "Linker.hpp"
#include "Logger.hpp"
#include <iostream>

AllData::AllData()
  : m_outputs(std::vector<OutputItem>()),
    m_outputGroups(std::vector<OutputGroup>()),
    m_importedLibs(std::vector<ImportedLib>()),
    m_buildTargets(std::vector<BuildTarget>())
{
  const std::string jsonFilePath = FileHelper::resolveFromRoot(Globals::JSON_FILE_NAME);

  JsonValue jsonData = JsonParser(jsonFilePath).getJsonCopy();
  loadOutputs(jsonData);
  loadGroups(jsonData);
  loadImportedLibs(jsonData);
  loadBuildTargets(jsonData);
  m_generalData = GeneralData(jsonData, m_buildTargets);
  validateLoadedItems();

  if (jsonData.hasOwnProperty(Tags::LINKS)) {
    linkLibrariesToOutputs(jsonData[Tags::LINKS].asMap(), m_outputGroups, m_outputs, m_importedLibs);
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

    // Correctly associate groups with their contained output items
    for (OutputGroup& group : m_outputGroups) {
      for (OutputItem& output : group.outputs()) {
        output.associateWithGroup(&group);
      }
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

void AllData::loadBuildTargets(JsonValue& jsonData) {
  if (jsonData.hasOwnProperty(Tags::BUILD_TARGETS)) {
    for (auto& [name, targetData] : jsonData[Tags::BUILD_TARGETS].asMap()) {
      m_buildTargets.emplace_back(name, targetData);
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

std::vector<OutputItem*> AllData::getExesPartOfLinkTree(ImportedLib& libChecking) {
  std::vector<OutputItem*> exesLinkedTo;

  for (OutputItem& output : m_outputs) {
    if (output.isExeType() && output.isPartOfImportedLibLinkTree(libChecking)) {
      exesLinkedTo.push_back(&output);
    }
  }

  for (OutputGroup& group : m_outputGroups) {
    for (OutputItem& output : group.outputs()) {
      if (output.isExeType() && output.isPartOfImportedLibLinkTree(libChecking)) {
        exesLinkedTo.push_back(&output);
      }
    }
  }
  return exesLinkedTo;
}

bool AllData::hasIndividualOutputsOfType(OutputType type) {
  for (OutputItem& output : m_outputs) {
    if (type == OutputType::EXE && output.isExeType())
      return true;
    else if (type == OutputType::SHARED_LIB && output.isSharedLibType())
      return true;
    else if (type == OutputType::STATIC_LIB && output.isStaticLibType())
      return true;
  }
  return false;
}

bool AllData::hasLibraryThatCanBeToggled() {
  for (OutputGroup& group : m_outputGroups) {
    if (group.hasLibraryThatCanBeToggled()) {
      return true;
    }
  }

  for (OutputItem& output : m_outputs) {
    if (output.canToggleLibraryType()) {
      return true;
    }
  }
  return false;
}

bool AllData::hasLibOutputGroups() {
  for (OutputGroup& group : m_outputGroups) {
    if (group.isLibraryType()) {
      return true;
    }
  }
  return false;
}

bool AllData::hasLinkedLibs() {
  for (OutputItem& output : m_outputs) {
    if (output.hasLinkedLibs()) {
      return true;
    }
  }

  for (OutputGroup& group : m_outputGroups) {
    if (group.hasLinkedLibs()) {
      return true;
    }

    for (OutputItem& output : group.outputs()) {
      if (output.hasLinkedLibs()) {
        return true;
      }
    }
  }
  return false;
}

bool AllData::hasExeOutputGroups() {
  for (OutputGroup& group : m_outputGroups) {
    if (group.isExeType()) {
      return true;
    }
  }
  return false;
}

bool AllData::anyOutputsDefined() {
  return !(m_outputs.empty() && m_outputGroups.empty());
}