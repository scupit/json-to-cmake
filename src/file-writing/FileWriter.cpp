#include "file-writing/FileWriter.hpp"

FileWriter::FileWriter(AllData& projectData, const std::string& fileNameWriting)
  : cmakeLists(std::ofstream(fileNameWriting)),
    data(&projectData),
    filePath(fileNameWriting)
{
  write();
}

FileWriter::~FileWriter() {
  if (cmakeLists.is_open()) {
    cmakeLists.close();
  }
}

void FileWriter::write() {
  if (!cmakeLists.is_open()) {
    cmakeLists.open(filePath);
  }

  cmakeLists.close();
}

// GENERAL WRITE FUNCTIONS;

void FileWriter::headerComment(const std::string& title) {
  cmakeLists << "# ////////////////////////////////////////////////////////////////////////////////"
    << std::endl << "# " << title
    << std::endl << "# ////////////////////////////////////////////////////////////////////////////////";
  newlines(2);
}

void FileWriter::itemLabel(const std::string& label) {
  cmakeLists << "# " << label << std::endl;
}

void FileWriter::newlines(const unsigned int numNewlines) {
  for (unsigned i = 0; i < numNewlines; ++i) {
    cmakeLists << std::endl; 
  }
}

// PROJECT WRITE FUNCTIONS

// void writeWatermark();
// void writeCmakeVersion();
// void writeProjectName();
// void writeCustomCmakeFunctions();
// void writeImportedLibs();

// void writeStandards();
// void writeBuildTargets();

// void libCreationFunctionString(const OutputItem&);
// void writeGeneralOutputData(OutputItem&);
// void writeSharedLib(OutputItem&);
// void writeStaticLib(OutputItem&);
// void writeExe(OutputItem&);
// void writeOutputDirs(OutputItem&);
// void writeOutputs();

// void writeGeneralGroupData(OutputGroup&);
// void writeLibraryGroup(OutputGroup&);
// void writeExeGroup(OutputGroup&);
// void writeOutputGroups();

// void writeLinksForItem(const std::string&, const std::vector<OutputItem*>, const std::vector<ImportedLib*>&);
// void writeLinks();

// void writeImportedLibCopyCommands();
