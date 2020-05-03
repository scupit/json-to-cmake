#include "output/OutputItem.hpp"

#include <stdexcept>
#include "helpers/FileHelper.hpp"
#include "Logger.hpp"
#include "constants/Tags.hpp"
#include "output/OutputGroup.hpp"

const std::string OutputItem::exeOutputDir = "bin";
const std::string OutputItem::libOutputDir = "lib";
const std::string OutputItem::archiveOutputDir = "lib";

OutputItem::OutputItem(const std::string& name, JsonValue& outputItemData, OutputGroup* groupContainedIn)
  : OutputBase(name, outputItemData),
    m_mainFile(std::string()),
    groupContainedIn(groupContainedIn)
{
  try {
    loadType(outputItemData);
  }
  catch (std::logic_error& e) {
    logErrorThenQuit(e.what());
  }

  loadCanToggleType(outputItemData);
  loadMainFile(outputItemData);
}

OutputItem::~OutputItem() { }

void OutputItem::loadType(JsonValue& outputItemData) {
  loadTypeBase(outputItemData, "Output item");
}

void OutputItem::loadMainFile(JsonValue& outputItemData) {
  if (isExeType() && outputItemData.hasOwnProperty(Tags::MAIN_FILE)) {
    m_mainFile = FileHelper::normalizeAbsolutePath(FileHelper::resolveFromRoot(outputItemData[Tags::MAIN_FILE].asString()));
  }
}

bool OutputItem::parentGroupHasHeaders() const {
  return isContainedInGroup() && groupContainedIn->hasOrInheritsHeaders();
}

bool OutputItem::parentGroupHasIncludeDirs() const {
  return isContainedInGroup() && groupContainedIn->hasOrInheritsIncludeDirs();
}

bool OutputItem::hasOrInheritsHeaders() const {
  return OutputBase::hasOrInheritsHeaders() || parentGroupHasHeaders();
}

bool OutputItem::hasOrInheritsIncludeDirs() const {
  return OutputBase::hasOrInheritsIncludeDirs() || parentGroupHasIncludeDirs();
}

bool OutputItem::isPartOfImportedLibLinkTree(const ImportedLib& libToCheck) const {
  return OutputBase::isPartOfImportedLibLinkTree(libToCheck) || (isContainedInGroup() && groupContainedIn->isPartOfImportedLibLinkTree(libToCheck));
}