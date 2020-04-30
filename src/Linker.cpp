#include "Linker.hpp"

#include "helpers/StringHelper.hpp"

// void linkLibrariesToOutputs(JsonValue&, std::vector<OutputGroup>&, std::vector<OutputItem>&, std::vector<ImportedLib>&);
// void linkToOutput(const std::vector<std::string>&, std::vector<OutputGroup>&, std::vector<OutputItem>&, std::vector<ImportedLib>&, OutputBase&, const std::string&, const std::string&);

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

  if (outputNameSearching.find(".") == std::string::npos) {
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