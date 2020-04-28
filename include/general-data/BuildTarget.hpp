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

		void loadCompilerFlags(JsonValue&);
		void loadCompilerDefinitions(JsonValue&);

	public:
		BuildTarget(const std::string&, JsonValue&);
		~BuildTarget();

		const std::string& name() const { return m_name; } 
		const std::vector<std::string>& compilerFlags() const { return m_compilerFlags; }
		const std::vector<std::string> compilerDefines() const { return m_compilerDefinitions; }

		inline bool hasCompilerFlags() const { return !m_compilerFlags.empty(); }
		inline bool hasCompileDefinitions() const { return !m_compilerDefinitions.empty(); }
};

#endif