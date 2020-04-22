#ifndef ALLDATA_HPP
#define ALLDATA_HPP

#include "JSON/JsonValue.hpp"
#include "output/OutputItem.hpp"
#include "output/OutputGroup.hpp"
#include <vector>

class AllData {
	private:
		std::vector<OutputItem> m_outputs;
		std::vector<OutputGroup> m_outputGroups;

		void loadOutputs(JsonValue&);
		void loadGroups(JsonValue&);

	public:
		AllData();
		~AllData();

		inline std::vector<OutputItem>& outputs() { return m_outputs; }
		inline std::vector<OutputGroup>& outputGroups() { return m_outputGroups; }
};

#endif