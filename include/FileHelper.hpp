#ifndef FILEHELPER_HPP
#define FILEHELPER_HPP

#include <filesystem>
#include <string>
#include <vector>

namespace FileHelper {
	std::string asPosix(const std::filesystem::path&);
	std::string asPosix(std::string);
  std::string joinPath(const std::vector<std::string>&);
	std::filesystem::path projectRoot(void);
	std::string resolve(const std::string&, const std::string&);
	std::string resolve(const std::filesystem::path&, const std::string&);
	std::string resolveFromRoot(const std::string&);
  std::vector<std::string> splitPath(const std::string&);
}

#endif