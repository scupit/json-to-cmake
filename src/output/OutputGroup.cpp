#include "output/OutputGroup.hpp"

#include "Logger.hpp"

OutputGroup::OutputGroup(const std::string& name, JsonValue& outputItemData)
  : OutputBase(name, outputItemData)
{
  try {
    loadType(outputItemData);
  }
  catch (std::logic_error& e) {
    logErrorThenQuit(e.what());
  }
  loadCanToggleType(outputItemData);
}

OutputGroup::~OutputGroup() {

}

void OutputGroup::loadType(JsonValue& outputItemData) {
  loadTypeBase(outputItemData, "Output Group");
}