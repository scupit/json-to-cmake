#ifndef IMPORTEDLIB_HPP
#define IMPORTEDLIB_HPP

#include <string>

class ImportedLib {
	private:
		const std::string m_name;

	public:
		ImportedLib();
		~ImportedLib();

		inline const std::string& name() const { return m_name; }

		bool hasHeaders() const;
		bool hasIncludeDirs() const;
};

#endif