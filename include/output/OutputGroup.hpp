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
		void loadOutputs(JsonValue&);

		bool isOutputItemCompatible(const OutputItem&) const;

	public:
		OutputGroup(const std::string&, JsonValue&);
		~OutputGroup();

		inline const std::vector<OutputItem>& outputs() const { return m_outputs; }

		std::string getPrefixedName() const;
		bool hasLibraryThatCanBeToggled() const;
};

#endif