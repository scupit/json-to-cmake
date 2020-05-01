#ifndef OUTPUTBASE_HPP
#define OUTPUTBASE_HPP

#include <set>
#include <string>
#include <vector>
#include "JSON/JsonValue.hpp"
#include "ImportedLib.hpp"

class OutputGroup;
class OutputItem;

enum class OutputType {
	NOT_SPECIFIED,
	EXE,
	SHARED_LIB,
	STATIC_LIB
};

class OutputBase {
	protected:
		const std::string m_name;

		std::vector<OutputItem*> linkedOutputs;
		std::vector<ImportedLib*> linkedImportedLibs;
		std::vector<OutputGroup*> linkedGroups;

		bool m_canToggleLibraryType;

		OutputType m_type;

		std::set<std::string> m_headers;
		std::set<std::string> m_sources;
		std::set<std::string> m_includeDirs;

		// Functions
		void loadTypeBase(JsonValue&, const std::string&);
		virtual void loadType(JsonValue&) = 0;
		void loadCanToggleType(JsonValue&);
		void getAllOutputsFromLinkedGroups(std::vector<OutputItem*>&);

	public:
		OutputBase(const std::string&, JsonValue&);
		virtual ~OutputBase();

		inline const std::string& name() const { return m_name; }

		inline const std::set<std::string>& headers() const { return m_headers; }
		inline const std::set<std::string>& sources() const { return m_sources; }
		inline const std::set<std::string>& includeDirs() const { return m_includeDirs; }

		const std::vector<OutputItem*>& localLinkedOutputs() const { return linkedOutputs; }
		const std::vector<OutputGroup*>& localLinkedGroups() const { return linkedGroups; }
		const std::vector<ImportedLib*>& localLinkedImports() const { return linkedImportedLibs; }

		inline bool canToggleLibraryType() const { return m_canToggleLibraryType; }
		inline bool isExeType() const { return m_type == OutputType::EXE; }
		inline bool isSharedLibType() const { return m_type == OutputType::SHARED_LIB; }
		inline bool isStaticLibType() const { return m_type == OutputType::STATIC_LIB; }
		inline bool isLibraryType() const { return isSharedLibType() || isStaticLibType(); }

		virtual bool hasLinkedLibs() const;
		virtual bool hasOrInheritsHeaders() const;
		virtual bool hasOrInheritsIncludeDirs() const;

		void getAllLinkedOutputs(std::vector<OutputItem*>&);
		void getAllLinkedImportedLibs(std::vector<ImportedLib*>&);

		void linkLib(ImportedLib*);
		void linkLib(OutputItem*);
		void linkGroup(OutputGroup*);

		virtual bool isPartOfImportedLibLinkTree(const ImportedLib&) const;
		std::string getTypeString() const;
};

#endif