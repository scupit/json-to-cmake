#include "output/OutputItem.hpp"

#include <stdexcept>
#include "Logger.hpp"

OutputItem::OutputItem(const std::string& name, JsonValue& outputItemData)
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

OutputItem::~OutputItem() { }

void OutputItem::loadType(JsonValue& outputItemData) {
  loadTypeBase(outputItemData, "Output item");
}