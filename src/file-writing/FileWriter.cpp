#include "file-writing/FileWriter.hpp"

#include "file-writing/CmakeCustomFunctions.hpp"
#include "helpers/FileWriteHelper.hpp"

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

void FileWriter::writeWatermark() {
  cmakeLists << "################################################################################" << std::endl
             << "# Generated with Skylar Cupit's json-to-cmake tool" << std::endl
             << "################################################################################";
}

void FileWriter::writeCmakeVersion() {
  cmakeLists << "cmake_minimum_required( VERSION " << data->generalData().cmakeVersion << std::endl
             << "                        LANGUAGES C CXX )";
}

void FileWriter::writeProjectName() {
  cmakeLists << "project( " << data->generalData().projectName << " )";
}

void FileWriter::writeCustomCmakeFunctions() {
  if (data->hasLibraryThatCanBeToggled()) {
    headerComment("Custom Functions");
    cmakeLists << CmakeCustomFunctions::TOGGLEABLE_LIBRARY_CREATOR << std::endl;
  }
}

void FileWriter::writeImportedLibs() {
  headerComment("IMPORTED LIBRARIES");

  for (ImportedLib& lib : data->importedLibs()) {
    itemLabel("Imported library: " + lib.name());

    if (lib.hasIncludeDirs()) {
      cmakeLists << "set( " << includeDirsVariable(lib.name());

      for (const std::string& includeDir : lib.includeDirs()) {
        cmakeLists << "\n\t" PROJECT_SOURCE_DIR << '/' << includeDir;
      }
      cmakeLists << "\n)";
      newlines(2);
    }

    if (lib.hasHeaders()) {
      cmakeLists << "set( " << headersVariable(lib.name());

      for (const std::string& headerFile : lib.headers()) {
        cmakeLists << "\n\t" PROJECT_SOURCE_DIR << '/' << headerFile;
      }
      cmakeLists << "\n)";
    }

    for (const std::string& libFileName : lib.libFiles()) {
      newlines(2);
      cmakeLists << "find_library( " << mangleLibName(lib.name(), libFileName)
        << "\n\tNAMES " << libFileName
        << "\n\tPATHS " PROJECT_SOURCE_DIR "/" << lib.dirContainingLibFiles()
        << "\n)";
    }
  }
}

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
