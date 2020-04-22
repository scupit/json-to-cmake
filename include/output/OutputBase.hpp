#ifndef OUTPUTBASE_HPP
#define OUTPUTBASE_HPP

#include <set>
#include <string>
#include <vector>
#include "JSON/JsonValue.hpp"

class ImportedLib;
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
		std::string name;

		std::vector<OutputItem*> linkedOutputs;
		std::vector<ImportedLib*> linkedImportedLibs;
		std::vector<OutputGroup*> linkedGroups;

		bool canToggleLibraryType;

		OutputType m_type;

		std::set<std::string> m_headers;
		std::set<std::string> m_sources;
		std::set<std::string> m_includeDirs;

		// Functions
		void loadTypeBase(JsonValue&, const std::string&);
		virtual void loadType(JsonValue&) = 0;

	public:
		OutputBase(const std::string&, JsonValue&);
		virtual ~OutputBase();

		inline std::set<std::string>& headers() { return m_headers; }
		inline std::set<std::string>& sources() { return m_sources; }
		inline std::set<std::string>& includeDirs() { return m_includeDirs; }

		inline bool isExeType() { return m_type == OutputType::EXE; }
		inline bool isSharedLibType() { return m_type == OutputType::SHARED_LIB; }
		inline bool isStaticLibType() { return m_type == OutputType::STATIC_LIB; }
		inline bool isLibraryType() { return isSharedLibType() || isStaticLibType(); }

	private:
		void loadCanToggleType(JsonValue&);
};

#endif