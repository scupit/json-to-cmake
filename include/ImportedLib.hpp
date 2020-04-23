#ifndef IMPORTEDLIB_HPP
#define IMPORTEDLIB_HPP

#include "JSON/JsonValue.hpp"
#include <string>
#include <set>
#include <vector>

class ImportedLib {
	private:
		const std::string m_name;

		std::set<std::string> m_headers;
		std::set<std::string> m_includeDirs;

		std::string m_generatedDepDirname;

		std::string m_gitRepoToClone;
		bool m_shouldCloneRepo;

		std::string m_dirContainingLibraryFiles;
		std::vector<std::string> m_libraryFileNames;

		void loadRootDir(JsonValue&);
		void loadLibraryFiles(JsonValue&);
		void loadGitRepoToClone(JsonValue&);
		void loadGeneratedDirname(JsonValue&);

	public:
		ImportedLib(const std::string&, JsonValue&);
		~ImportedLib();

		const std::string& name() const { return m_name; }
		const std::set<std::string>& headers() { return m_headers; }
		const std::set<std::string>& includeDirs() { return m_includeDirs; }
		const std::vector<std::string>& libFiles() { return m_libraryFileNames; }

		inline bool hasHeaders() const { return !m_headers.empty(); }
		inline bool hasIncludeDirs() const { return !m_includeDirs.empty(); }
		inline bool shouldCloneRepo() const { return m_shouldCloneRepo; }
		inline bool hasRepoToClone() const { return !m_gitRepoToClone.empty(); }
};

#endif