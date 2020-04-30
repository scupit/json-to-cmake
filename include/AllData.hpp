#ifndef ALLDATA_HPP
#define ALLDATA_HPP

#include "JSON/JsonValue.hpp"
#include "general-data/BuildTarget.hpp"
#include "general-data/GeneralData.hpp"
#include "output/OutputItem.hpp"
#include "output/OutputGroup.hpp"
#include "ImportedLib.hpp"
#include <vector>

class AllData {
	private:
		std::vector<OutputItem> m_outputs;
		std::vector<OutputGroup> m_outputGroups;
		std::vector<ImportedLib> m_importedLibs;

		std::vector<BuildTarget> m_buildTargets;
		GeneralData m_generalData;

		void loadOutputs(JsonValue&);
		void loadGroups(JsonValue&);
		void loadImportedLibs(JsonValue&);
		void loadBuildTargets(JsonValue&);
		void validateLoadedItems();

	public:
		AllData();
		~AllData();

		std::vector<OutputItem>& outputs() { return m_outputs; }
		std::vector<OutputGroup>& outputGroups() { return m_outputGroups; }
		std::vector<ImportedLib>& importedLibs() { return m_importedLibs; }
		std::vector<BuildTarget>& buildTargets() { return m_buildTargets; }
		GeneralData& generalData() { return m_generalData; }

		bool anyOutputsDefined();
};

#endif