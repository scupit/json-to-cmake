#include "Linker.hpp"

#include "Logger.hpp"
#include "helpers/StringHelper.hpp"

void linkLibrariesToOutputs(
    JsonObject& linkData,
    std::vector<OutputGroup>& outputGroups,
    std::vector<OutputItem>& outputItems,
    std::vector<ImportedLib>& importedLibs) {

  for (auto& [nameLinkingTo, jsonNamesLinkingTo] : linkData) {
    OutputItem* outputLinkingTo = getOutputItemByName(outputGroups, outputItems, nameLinkingTo);
    OutputGroup* groupLinkingTo = getOutputGroupByName(outputGroups, nameLinkingTo);

    std::vector<std::string> linkedLibNames;
    for (JsonValue& jsonName : jsonNamesLinkingTo.asVector()) {
      linkedLibNames.push_back(jsonName.asString());
    }

    if (outputLinkingTo) {
      linkToOutput(linkedLibNames, outputGroups, outputItems, importedLibs, *outputLinkingTo, nameLinkingTo, "output library");
    }
    else if (groupLinkingTo) {
      linkToOutput(linkedLibNames, outputGroups, outputItems, importedLibs, *groupLinkingTo, nameLinkingTo, "output library group");
    }
    else {
      logErrorThenQuit(std::string("Tried linking to output or group \"") + nameLinkingTo + "\" which does not exist.");
    }
  }
}

void linkToOutput(
    const std::vector<std::string>& namesLinking,
    std::vector<OutputGroup>& outputGroups,
    std::vector<OutputItem>& outputItems,
    std::vector<ImportedLib>& importedLibs,
    OutputBase& outputLinkingTo,
    const std::string& fullNameLinkingTo,
    const std::string& itemTypeString) {

  for (const std::string& linkedLibName : namesLinking) {
    OutputItem* outputLinkingFrom = getOutputItemByName(outputGroups, outputItems, linkedLibName);
    OutputGroup* groupLinkingFrom = getOutputGroupByName(outputGroups, linkedLibName);
    ImportedLib* importedLibLinkingFrom = getImportedLibByName(importedLibs, linkedLibName);

    if (outputLinkingFrom) {
      if (outputLinkingFrom->isExeType()) {
        logErrorThenQuit("Cannot link executable output " + linkedLibName + " to " + fullNameLinkingTo);
      }
      else if (outputLinkingTo.isLibraryType()) {
        logErrorThenQuit("Please don't link output library \"" + linkedLibName + "\" to another " + itemTypeString + " (" + linkedLibName + ")");
      }
      else {
        outputLinkingTo.linkLib(outputLinkingFrom);
      }
    }
    else if (groupLinkingFrom) {
      if (groupLinkingFrom->isExeType()) {
        logErrorThenQuit("Cannot link exe type group " + linkedLibName + " to " + fullNameLinkingTo);
      }
      else if (outputLinkingTo.isLibraryType()) {
        logErrorThenQuit("Please don't link output type library group \"" + linkedLibName + "\" to another " + itemTypeString + " (" + fullNameLinkingTo + ")");
      }
      else {
        outputLinkingTo.linkGroup(groupLinkingFrom);
      }
    }
    else if (importedLibLinkingFrom) {
      outputLinkingTo.linkLib(importedLibLinkingFrom);
    }
    else {
      logErrorThenQuit("Cannot link nonexistend library or group \"" + linkedLibName + "\" to " + fullNameLinkingTo);
    }
  }
}

OutputGroup* getOutputGroupByName(std::vector<OutputGroup>& outputGroups, const std::string& groupNameSearching) {
  if (groupNameSearching.find(".") == std::string::npos) {
    for (OutputGroup& group : outputGroups) {
      if (group.name() == groupNameSearching) {
        return &group;
      }
    } 
  }
  return nullptr;
}

OutputItem* getOutputItemByName(std::vector<OutputGroup>& outputGroups, std::vector<OutputItem>& outputItems, const std::string& outputNameSearching) {

  if (outputNameSearching.find(".") != std::string::npos) {
    std::vector<std::string> nameSections = StringHelper::split(outputNameSearching, ".");
    const std::string
      &groupName = nameSections[0],
      &actualOutputName = nameSections[1];

    OutputGroup* containingGroup = getOutputGroupByName(outputGroups, groupName);
    if (containingGroup) {
      for (OutputItem& output : containingGroup->outputs()) {
        if (output.name() == actualOutputName) {
          return &output;
        }
      }
    }
  }
  else {
    for (OutputItem& output : outputItems) {
      if (output.name() == outputNameSearching) {
        return &output;
      }
    }
  }

  return nullptr;
}

ImportedLib* getImportedLibByName(std::vector<ImportedLib>& importedLibs, const std::string& importedLibNameSearching) {
  for (ImportedLib& importedLib : importedLibs) {
    if (importedLib.name() == importedLibNameSearching) {
      return &importedLib;
    }
  }
  return nullptr;
}