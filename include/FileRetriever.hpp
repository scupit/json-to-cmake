#ifndef FILERETRIEVER_HPP
#define FILERETRIEVER_HPP

#include <string>
#include <set>

#include "JSON/JsonValue.hpp"

namespace FileRetriever {
	void loadSourceFiles(std::set<std::string>&, JsonValue&);
	void loadHeaderFiles(std::set<std::string>&, JsonValue&);
	void loadIncludeDirs(std::set<std::string>&, JsonValue&);
}

#endif