#ifndef FILEWRITER_HPP
#define FILEWRITER_HPP

#include "AllData.hpp"
#include "ImportedLib.hpp"
#include "output/OutputGroup.hpp"
#include "output/OutputItem.hpp"
#include <fstream>
#include <string>
#include <vector>

class FileWriter {
	private:
		AllData* data;
		std::ofstream cmakeLists;
		std::string filePath;

		void headerComment(const std::string&);
		void itemLabel(const std::string&);
		void newlines(const unsigned int = 1);
		std::string libCreationFunctionString(const OutputItem&);
		void labelOutput(OutputItem&);
		
		void writeOutputDirs(OutputItem&);
		void writeWatermark();
		void writeCmakeVersion();
		void writeProjectName();
		void writeCustomCmakeFunctions();
		void writeImportedLibs();

		void writeStandards();
		void writeBuildTargets();

		void writeGeneralOutputData(OutputItem&);
		void writeSharedLib(OutputItem&);
		void writeStaticLib(OutputItem&);
		void writeExe(OutputItem&);
		void writeOutputs();

		void writeGeneralGroupData(OutputGroup&);
		void writeLibraryGroup(OutputGroup&);
		void writeExeGroup(OutputGroup&);
		void writeOutputGroups();

		void writeLinksForItem(const std::string&, const std::vector<OutputItem*>, const std::vector<ImportedLib*>&);
		void writeLinks();

		void writeImportedLibCopyCommands();

	public:
		FileWriter(AllData&, const std::string&);
		~FileWriter();

		void write();
};

#endif