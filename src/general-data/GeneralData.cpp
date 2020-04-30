#include "general-data/GeneralData.hpp"

#include "constants/Globals.hpp"
#include "constants/Tags.hpp"
#include "helpers/FileHelper.hpp"
#include "Logger.hpp"

// Assume at least one build target already exists
GeneralData::GeneralData(JsonValue& jsonData, std::vector<BuildTarget>& buildTargets)
  : cmakeVersion(Globals::CMAKE_VERSION),
    supportedCppStandards(std::vector<std::string>()),
    supportedCStandards(std::vector<std::string>())
{
  loadProjectName(jsonData);
  loadStandards(jsonData);
  loadDefaultBuildTarget(jsonData, buildTargets);
}

GeneralData::~GeneralData() { }

bool GeneralData::hasStandard(const std::vector<std::string>& standards, const std::string& stringSearching) {
  for (const std::string& standard : standards) {
    if (standard == stringSearching) {
      return true;
    }
  }
  return false;
}

void GeneralData::loadProjectName(JsonValue& jsonData) {
  projectName = jsonData.hasOwnProperty(Tags::PROJECT_NAME)
    ? jsonData[Tags::PROJECT_NAME].asString()
    : FileHelper::getProjectName();
}

void GeneralData::loadStandards(JsonValue& jsonData) {
  bool cStandardsDefinedByUser = false;
  bool cppStandardsDefinedByUser = false;

  if (jsonData.hasOwnProperty(Tags::C_STANDARDS)) {
    for (JsonValue& standard : jsonData[Tags::C_STANDARDS].asVector()) {
      supportedCStandards.push_back(standard.asString());
    }
    cStandardsDefinedByUser = true;
  }
  else {
    supportedCStandards.push_back(Globals::DEFAULT_C_STANDARD);
  }

  if (jsonData.hasOwnProperty(Tags::CPP_STANDARDS)) {
    for (JsonValue& standard : jsonData[Tags::CPP_STANDARDS].asVector()) {
      supportedCppStandards.push_back(standard.asString());
    }
    cppStandardsDefinedByUser = true;
  }
  else {
    supportedCppStandards.push_back(Globals::DEFAULT_CPP_STANDARD);
  }

  loadDefaultStandards(jsonData, cStandardsDefinedByUser, cppStandardsDefinedByUser);
}

void GeneralData::loadDefaultStandards(JsonValue& jsonData, const bool cStandardsDefinedByUser, const bool cppStandardsDefinedByUser) {
  if (jsonData.hasOwnProperty(Tags::DEFAULT_C_STANDARD)) {
    if (!cStandardsDefinedByUser) {
      supportedCStandards[0] = jsonData[Tags::DEFAULT_C_STANDARD].asString();
    }

    if (hasStandard(supportedCStandards, jsonData[Tags::DEFAULT_C_STANDARD].asString())) {
      defaultCStandard = jsonData[Tags::DEFAULT_C_STANDARD].asString();
    }
    else {
      logErrorThenQuit(std::string("The default C Standard must be present in ") + Tags::C_STANDARDS);
    }
  }
  else {
    defaultCStandard = supportedCStandards[0];
  }

  if (jsonData.hasOwnProperty(Tags::DEFAULT_CPP_STANDARD)) {
    if (!cppStandardsDefinedByUser) {
      supportedCppStandards[0]= jsonData[Tags::DEFAULT_CPP_STANDARD].asString();
    }

    if (hasStandard(supportedCppStandards, jsonData[Tags::DEFAULT_CPP_STANDARD].asString())) {
      defaultCppStandard = jsonData[Tags::DEFAULT_CPP_STANDARD].asString();
    }
    else {
      logErrorThenQuit(std::string("The default C++ standard must be present in ") + Tags::CPP_STANDARDS);
    }
  }
  else {
    defaultCppStandard = supportedCppStandards[0];
  }
}

void GeneralData::loadDefaultBuildTarget(JsonValue& jsonData, std::vector<BuildTarget>& buildTargets) {
  if (jsonData.hasOwnProperty(Tags::DEFAULT_BUILD_TARGET)) {
    for (BuildTarget& target : buildTargets) {
      if (target.name() == jsonData[Tags::DEFAULT_BUILD_TARGET]) {
        defaultBuildTarget = &target;
        return;
      }
    }

    logErrorThenQuit(std::string(Tags::DEFAULT_BUILD_TARGET) + " must exist in " + Tags::BUILD_TARGETS);
  }
  else {
    defaultBuildTarget = &buildTargets[0];
  }
}