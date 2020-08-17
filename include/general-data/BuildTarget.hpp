#ifndef BUILDTARGET_HPP
#define BUILDTARGET_HPP

#include <string>
#include <vector>
#include "JSON/JsonValue.hpp"

class BuildTarget {
	private:
		std::string m_name;
		std::vector<std::string> m_compilerFlags;
		std::vector<std::string> m_compilerDefinitions;

		std::vector<std::string> m_otherFiles;
		std::vector<std::string> m_otherDirs;

		void loadCompilerFlags(JsonValue&);
		void loadCompilerDefinitions(JsonValue&);
		void loadOtherFiles(JsonValue&);
		void loadOtherDirs(JsonValue&);

	public:
		BuildTarget(const std::string&, JsonValue&);
		~BuildTarget();

		const std::string& name() const { return m_name; } 
		const std::vector<std::string>& compilerFlags() const { return m_compilerFlags; }
		const std::vector<std::string>& compilerDefines() const { return m_compilerDefinitions; }
		const std::vector<std::string>& otherFiles() const { return m_otherFiles; }
		const std::vector<std::string>& otherDirs() const { return m_otherDirs; }

		inline bool hasCompilerFlags() const { return !m_compilerFlags.empty(); }
		inline bool hasCompileDefinitions() const { return !m_compilerDefinitions.empty(); }
		inline bool hasOtherFiles() const { return !m_otherFiles.empty(); }
		inline bool hasOtherDirs() const { return !m_otherDirs.empty(); }
};

#endif