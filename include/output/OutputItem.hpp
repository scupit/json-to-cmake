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
		std::string mainFile;
		OutputGroup* groupContainedIn;

		void loadType(JsonValue&) override;
		void loadMainFile(JsonValue&);

	public:
		OutputItem(const std::string&, JsonValue&, OutputGroup* = nullptr);
		~OutputItem();

		inline bool isContainedInGroup() const { return groupContainedIn != nullptr; }

		bool parentGroupHasHeaders() const;
		bool parentGroupHasIncludeDirs() const;

		bool hasOrInheritsHeaders() const override;
		bool hasOrInheritsIncludeDirs() const override;

		bool isPartOfImportedLibLinkTree(const ImportedLib&) const override;
};

#endif