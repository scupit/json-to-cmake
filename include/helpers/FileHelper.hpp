#ifndef FILEHELPER_HPP
#define FILEHELPER_HPP

#include <filesystem>
#include <functional>
#include <string>
#include <vector>

namespace FileHelper {
	// File name formatting and resolution
	std::string asPosix(const std::filesystem::path&);
	std::string asPosix(std::string);
  std::string joinPath(const std::vector<std::string>&);
	std::filesystem::path projectRoot(void);
	std::string resolve(const std::string&, const std::string&);
	std::string resolve(const std::filesystem::path&, const std::string&);
	std::string resolveFromRoot(const std::string&);
  std::vector<std::string> splitPath(const std::string&);

	// File retrieval

	// T Should be some form of directory iterator
	template <class T>
	void getItemsThen(const std::string&, const std::function<void(const std::filesystem::path&)>&, const std::function<bool(const std::filesystem::directory_entry&)>&);

	void forFileInDir(const std::string&, const std::function<void(const std::filesystem::path&)>&);
	void forFileInDirRecursive(const std::string&, const std::function<void(const std::filesystem::path&)>&);
	void forDirRecursive(const std::string&, const std::function<void(const std::filesystem::path&)>&);

	// Helpers
	bool isFile(const std::filesystem::directory_entry&);
	bool isDir(const std::filesystem::directory_entry&);
}

template <class T>
void FileHelper::getItemsThen(const std::string& relativePath, const std::function<void(const std::filesystem::path&)>& callback, const std::function<bool(const std::filesystem::directory_entry&)>& predicate) {
  for (auto& entry : T(resolveFromRoot(relativePath))) {
    if (predicate(entry)) {
      callback(std::filesystem::path(asPosix(entry.path())));
    } 
  }
}

#endif