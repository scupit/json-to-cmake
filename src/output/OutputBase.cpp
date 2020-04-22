#include "output/OutputBase.hpp"

#include "output/OutputItem.hpp"
#include "output/OutputGroup.hpp"

#include <stdexcept>
#include "constants/Tags.hpp"
#include "FileRetriever.hpp"
#include "Logger.hpp"

OutputBase::OutputBase(const std::string& name, JsonValue& outputItemData)
  : name(name),
    linkedOutputs(std::vector<OutputItem*>()),
    linkedImportedLibs(std::vector<ImportedLib*>()),
    linkedGroups(std::vector<OutputGroup*>()),
    canToggleLibraryType(false),
    m_type(OutputType::NOT_SPECIFIED),
    m_headers(std::set<std::string>()),
    m_sources(std::set<std::string>()),
    m_includeDirs(std::set<std::string>())
{
  FileRetriever::loadHeaderFiles(m_headers, outputItemData);
  FileRetriever::loadSourceFiles(m_sources, outputItemData);
  FileRetriever::loadIncludeDirs(m_includeDirs, outputItemData);
  loadCanToggleType(outputItemData);
}

OutputBase::~OutputBase() { }

void OutputBase::loadTypeBase(JsonValue& outputItemData, const std::string& itemTypeString) {
  if (!outputItemData.hasOwnProperty(Tags::TYPE)) {
    throw std::logic_error(std::string(Tags::TYPE) + " missing from " + itemTypeString + name);
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
    canToggleLibraryType = outputItemData[Tags::LIB_TYPE_TOGGLE_POSSIBLE].asBool();
  }
}