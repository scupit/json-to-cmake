#ifndef GENERALDATA_HPP
#define GENERALDATA_HPP

#include <string>
#include <vector>
#include "JSON/JsonValue.hpp"
#include "general-data/BuildTarget.hpp"

class GeneralData {
	public:
		std::string cmakeVersion;
		std::string projectName;

		std::string defaultCppStandard;
		std::string defaultCStandard;

		std::vector<std::string> supportedCppStandards;
		std::vector<std::string> supportedCStandards;

		BuildTarget* defaultBuildTarget;	

		GeneralData() = default;
		GeneralData(JsonValue&, std::vector<BuildTarget>&);
		~GeneralData();

	private:
		bool hasStandard(const std::vector<std::string>&, const std::string&);
		void loadProjectName(JsonValue&);
		void loadStandards(JsonValue&);
		void loadDefaultStandards(JsonValue&, const bool, const bool);
		void loadDefaultBuildTarget(JsonValue&, std::vector<BuildTarget>&);
};

#endif