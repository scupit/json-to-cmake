#ifndef OUTPUTGROUP_HPP
#define OUTPUTGROUP_HPP

#include "JSON/JsonValue.hpp"
#include "output/OutputBase.hpp"
#include <string>
#include <vector>

class OutputGroup : public OutputBase{
	private:
		std::vector<OutputItem> m_outputs;	

		void loadType(JsonValue&) override;

	public:
		OutputGroup(const std::string&, JsonValue&);
		~OutputGroup();

		inline std::vector<OutputItem>& outputs() { return m_outputs; }
};

#endif