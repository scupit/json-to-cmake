#ifndef OUTPUTITEM_HPP
#define OUTPUTITEM_HPP

#include "JSON/JsonValue.hpp"
#include "output/OutputBase.hpp"
#include <string>

class OutputGroup;

class OutputItem : public OutputBase {
	public:
		static const std::string exeOutputDir;
		static const std::string libOutputDir;
		static const std::string archiveOutputDir;

	private:
		std::string m_mainFile;
		OutputGroup* groupContainedIn;

		void loadType(JsonValue&) override;
		void loadMainFile(JsonValue&);

	public:
		OutputItem(const std::string&, JsonValue&, OutputGroup* = nullptr);
		~OutputItem();

		void associateWithGroup(OutputGroup* group) { groupContainedIn = group; }

		inline bool isContainedInGroup() const { return groupContainedIn != nullptr; }
		const OutputGroup* containingGroup() const { return groupContainedIn; }
		const std::string& mainFile() const { return m_mainFile; }

		bool hasMainFile() const { return isExeType() && !m_mainFile.empty(); }
		bool parentGroupHasHeaders() const;
		bool parentGroupHasIncludeDirs() const;

		bool hasOrInheritsHeaders() const override;
		bool hasOrInheritsIncludeDirs() const override;

		bool isPartOfImportedLibLinkTree(const ImportedLib&) const override;
};

#endif