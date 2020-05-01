#ifndef LINKER_HPP
#define LINKER_HPP

#include "JSON/JsonValue.hpp"
#include "output/OutputBase.hpp"
#include "output/OutputGroup.hpp"
#include "output/OutputItem.hpp"
#include "ImportedLib.hpp"
#include <string>
#include <vector>

void linkLibrariesToOutputs(JsonObject&, std::vector<OutputGroup>&, std::vector<OutputItem>&, std::vector<ImportedLib>&);
void linkToOutput(const std::vector<std::string>&, std::vector<OutputGroup>&, std::vector<OutputItem>&, std::vector<ImportedLib>&, OutputBase&, const std::string&, const std::string&);
OutputGroup* getOutputGroupByName(std::vector<OutputGroup>&, const std::string&);
OutputItem* getOutputItemByName(std::vector<OutputGroup>&, std::vector<OutputItem>&, const std::string&);
ImportedLib* getImportedLibByName(std::vector<ImportedLib>&, const std::string&);

#endif