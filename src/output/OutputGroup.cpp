#include "output/OutputGroup.hpp"

#include "output/OutputItem.hpp"
#include "Logger.hpp"
#include "constants/Tags.hpp"
#include "constants/Globals.hpp"

OutputGroup::OutputGroup(const std::string& name, JsonValue& outputGroupData)
  : OutputBase(name, outputGroupData)
{
  try {
    loadType(outputGroupData);
    loadCanToggleType(outputGroupData);
    loadOutputs(outputGroupData);
  }
  catch (std::logic_error& e) {
    logErrorThenQuit(e.what());
  }
}

OutputGroup::~OutputGroup() { }

void OutputGroup::loadType(JsonValue& outputGroupData) {
  loadTypeBase(outputGroupData, "Output Group");
}

void OutputGroup::loadOutputs(JsonValue& outputGroupData) {
  if (!outputGroupData.hasOwnProperty(Tags::GROUP_OUTPUTS)) {
    logErrorThenQuit("Output Group " + m_name + " missing " + Tags::GROUP_OUTPUTS);
  }

  for (auto& [name, outputItemData] : outputGroupData[Tags::GROUP_OUTPUTS].asMap()) {
    if (!outputItemData.hasOwnProperty(Tags::TYPE)) {
      outputItemData[Tags::TYPE] = getTypeString();
    }

    if (!outputItemData.hasOwnProperty(Tags::LIB_TYPE_TOGGLE_POSSIBLE)) {
      outputItemData[Tags::LIB_TYPE_TOGGLE_POSSIBLE] = m_canToggleLibraryType;
    }

    // m_outputs.emplace_back(name, outputItemData, this);
    m_outputs.emplace_back(name, outputItemData);
  }

  if (m_outputs.empty()) {
    logErrorThenQuit("No outputs are defined for Group " + m_name);
  }

  for (const OutputItem& containedOutput : m_outputs) {
    if (!isOutputItemCompatible(containedOutput)) {
      logErrorThenQuit("Output " + containedOutput.name() + " in Group " + m_name + " has type " + containedOutput.getTypeString() + " which is incompatible with Group type " + getTypeString());
    }
  }
}

bool OutputGroup::isOutputItemCompatible(const OutputItem& containedOutput) const {
  return (isExeType() && containedOutput.isExeType()) || (isLibraryType() && containedOutput.isLibraryType());
}

std::string OutputGroup::getPrefixedName() const {
  return Globals::OUTPUT_GROUP_NAME_PREFIX + m_name;
}

bool OutputGroup::hasLibraryThatCanBeToggled() const {
  // Executable type groups will never contain a library output item.
  if (!isLibraryType())
    return false;

  for (const OutputItem& output : m_outputs) {
    if (output.canToggleLibraryType()) {
      return true;
    }
  } 
  return false;
}