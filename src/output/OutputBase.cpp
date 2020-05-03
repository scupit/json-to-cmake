#include "output/OutputBase.hpp"

#include "output/OutputItem.hpp"
#include "output/OutputGroup.hpp"

#include <stdexcept>
#include "constants/Tags.hpp"
#include "FileRetriever.hpp"
#include "Logger.hpp"

OutputBase::OutputBase(const std::string& name, JsonValue& outputItemData)
  : m_name(name),
    linkedOutputs(std::vector<OutputItem*>()),
    linkedImportedLibs(std::vector<ImportedLib*>()),
    linkedGroups(std::vector<OutputGroup*>()),
    m_canToggleLibraryType(false),
    m_type(OutputType::NOT_SPECIFIED),
    m_headers(std::set<std::string>()),
    m_sources(std::set<std::string>()),
    m_includeDirs(std::set<std::string>())
{
  FileRetriever::loadHeaderFiles(m_headers, outputItemData);
  FileRetriever::loadSourceFiles(m_sources, outputItemData);
  FileRetriever::loadIncludeDirs(m_includeDirs, outputItemData);
}

OutputBase::~OutputBase() { }

void OutputBase::loadTypeBase(JsonValue& outputItemData, const std::string& itemTypeString) {
  if (!outputItemData.hasOwnProperty(Tags::TYPE)) {
    throw std::logic_error(std::string(Tags::TYPE) + " missing from " + itemTypeString + m_name);
  }

  if (outputItemData[Tags::TYPE] == Tags::EXE) {
    m_type = OutputType::EXE;
  }
  else if (outputItemData[Tags::TYPE] == Tags::SHARED_LIB) {
    m_type = OutputType::SHARED_LIB;
  }
  else if (outputItemData[Tags::TYPE] == Tags::STATIC_LIB) {
    m_type = OutputType::STATIC_LIB;
  }
}

void OutputBase::loadCanToggleType(JsonValue& outputItemData) {
  if (isLibraryType() && outputItemData.hasOwnProperty(Tags::LIB_TYPE_TOGGLE_POSSIBLE)) {
    m_canToggleLibraryType = outputItemData[Tags::LIB_TYPE_TOGGLE_POSSIBLE].asBool();
  }
}

bool OutputBase::hasLinkedLibs() const {
  return !(linkedGroups.empty() && linkedImportedLibs.empty() && linkedOutputs.empty());
}

bool OutputBase::hasOrInheritsHeaders() const {
  for (const ImportedLib* linkedLib : linkedImportedLibs) {
    if (linkedLib->hasHeaders()) {
      return true;
    }
  }

  for (const OutputItem* linkedLib : linkedOutputs) {
    if (linkedLib->hasOrInheritsHeaders()) {
      return true;
    }
  }

  for (OutputGroup* linkedGroup : linkedGroups) {
    for (const OutputItem& output : linkedGroup->outputs()) {
      if (output.hasOrInheritsHeaders()) {
        return true;
      }
    } 

    if (linkedGroup->hasOrInheritsHeaders()) {
      return true;
    }
  }
  return !m_headers.empty();
}

bool OutputBase::hasOrInheritsIncludeDirs() const {
  for (const ImportedLib* linkedLib : linkedImportedLibs) {
    if (linkedLib->hasIncludeDirs()) {
      return true;
    }
  }

  for (const OutputItem* linkedLib : linkedOutputs) {
    if (linkedLib->hasOrInheritsIncludeDirs()) {
      return true;
    }
  }

  for (OutputGroup* linkedGroup : linkedGroups) {
    for (const OutputItem& output : linkedGroup->outputs()) {
      if (output.hasOrInheritsIncludeDirs()) {
        return true;
      }
    }

    if (linkedGroup->hasOrInheritsIncludeDirs()) {
      return true;
    }
  }
  return !m_includeDirs.empty();
}

void OutputBase::linkLib(ImportedLib* libToLink) {
  linkedImportedLibs.push_back(libToLink);
}

void OutputBase::linkLib(OutputItem* libToLink) {
  linkedOutputs.push_back(libToLink);
}

void OutputBase::linkGroup(OutputGroup* groupToLink) {
  linkedGroups.push_back(groupToLink);
}

void OutputBase::getAllOutputsFromLinkedGroups(std::vector<OutputItem*>& outputList) {
  for (OutputGroup* linkedGroup : linkedGroups) {
    if (linkedGroup->isLibraryType()) {
      for (OutputItem& output : linkedGroup->outputs()) {
        outputList.push_back(&output);
      }
    }
  }
}

std::vector<OutputItem*> OutputBase::getAllLinkedOutputs() {
  std::vector<OutputItem*> outputList;
  for (OutputItem* linkedOutput : linkedOutputs) {
    outputList.push_back(linkedOutput);
  }
  getAllOutputsFromLinkedGroups(outputList);
  return outputList;
}

std::vector<ImportedLib*> OutputBase::getAllLinkedImportedLibs() {
  return linkedImportedLibs;
}

bool OutputBase::isPartOfImportedLibLinkTree(const ImportedLib& importedLib) const {
  for (const ImportedLib* linkedLib : linkedImportedLibs) {
    if (importedLib.name() == linkedLib->name()) {
      return true;
    }
  }

  for (const OutputItem* linkedLib : linkedOutputs) {
    if (linkedLib->isPartOfImportedLibLinkTree(importedLib)) {
      return true;
    }
  }

  for (OutputGroup* linkedGroup : linkedGroups) {
    if (linkedGroup->isPartOfImportedLibLinkTree(importedLib)) {
      return true;
    }

    for (const OutputItem& output : linkedGroup->outputs()) {
      if (output.isPartOfImportedLibLinkTree(importedLib)) {
        return true;
      }
    }
  }
  return false;
}

std::string OutputBase::getTypeString() const {
  if (isSharedLibType())
    return Tags::SHARED_LIB;
  else if (isStaticLibType())
    return Tags::STATIC_LIB;
  return Tags::EXE;
}