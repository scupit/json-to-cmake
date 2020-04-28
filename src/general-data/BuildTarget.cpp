#include "general-data/BuildTarget.hpp"
#include "constants/Tags.hpp"

BuildTarget::BuildTarget(const std::string& name, JsonValue& buildTargetData)
  : m_name(name),
    m_compilerFlags(std::vector<std::string>()),
    m_compilerDefinitions(std::vector<std::string>())
{
  loadCompilerFlags(buildTargetData);
  loadCompilerDefinitions(buildTargetData);
}

BuildTarget::~BuildTarget() { }

void BuildTarget::loadCompilerFlags(JsonValue& buildTargetData) {
  if (buildTargetData.hasOwnProperty(Tags::COMPILER_FLAGS)) {
    for (JsonValue& flag : buildTargetData[Tags::COMPILER_FLAGS].asVector()) {
      m_compilerFlags.push_back(flag.asString());
    }
  }
}

void BuildTarget::loadCompilerDefinitions(JsonValue& buildTargetData) {
  if (buildTargetData.hasOwnProperty(Tags::COMPILER_DEFINES)) {
    for (JsonValue& definition : buildTargetData[Tags::COMPILER_DEFINES].asVector()) {
      m_compilerDefinitions.push_back(definition.asString()); 
    }
  }
}