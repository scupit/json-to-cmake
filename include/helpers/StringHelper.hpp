#ifndef STRINGHELPER_HPP
#define STRINGHELPER_HPP

#include <string>
#include <vector>

namespace StringHelper {
	std::vector<std::string> split(const std::string&, const std::string&);
	std::string join(const std::vector<std::string>&, const std::string&);
	std::string replaceAll(std::string, const std::string&, const std::string&);
}

#endif