#include "output/OutputItem.hpp"

#include <stdexcept>
#include "Logger.hpp"

const std::string OutputItem::exeOutputDir = "bin";
const std::string OutputItem::libOutputDir = "lib";
const std::string OutputItem::archiveOutputDir = "lib";

OutputItem::OutputItem(const std::string& name, JsonValue& outputItemData, OutputGroup* groupContainedIn)
  : OutputBase(name, outputItemData),
    groupContainedIn(groupContainedIn)
{
  try {
    loadType(outputItemData);
  }
  catch (std::logic_error& e) {
    logErrorThenQuit(e.what());
  }

  loadCanToggleType(outputItemData);
}

OutputItem::~OutputItem() { }

void OutputItem::loadType(JsonValue& outputItemData) {
  loadTypeBase(outputItemData, "Output item");
}