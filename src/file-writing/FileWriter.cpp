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

void FileWriter::writeStandards() {
  headerComment("LANGUAGE STANDARDS");

  // Default C standard
  cmakeLists << "set( CMAKE_C_STANDARD " << data->generalData().defaultCStandard << " CACHE STRING \"C compiler standard\" )";

  // Supported C standard
  cmakeLists << "\nset_property( CACHE CMAKE_C_STANDARD PROPERTY STRINGS ";
  for (const std::string& standard : data->generalData().supportedCStandards) {
    cmakeLists << standard << ' ';
  }
  cmakeLists << ')'
    << "\nmessage( \"Using C compiler standard --std=c${CMAKE_C_STANDARD}\" )";

  // Default C++ standard
  cmakeLists << "\n\nset( CMAKE_CXX_STANDARD " << data->generalData().defaultCppStandard << " CACHE STRING \"CXX compiler standard\" )" ;

  // Supported C++ standards
  cmakeLists << "\nset_property( CACHE CMAKE_CXX_STANDARD PROPERTY STRINGS ";
  for (const std::string& standard : data->generalData().supportedCppStandards) {
    cmakeLists << standard << ' ';
  }
  cmakeLists << ')'
    << "\nmessage( \"Using CXX compiler standard --std=c++${CMAKE_CXX_STANDARD}\" )";

  cmakeLists << "\n\nset( CMAKE_C_STANDARD_REQUIRED ON )"
    << "\nset( CMAKE_CXX_STANDARD_REQUIRED ON )"
    << "\n\nset( CMAKE_C_EXTENSIONS OFF )"
    << "\nset( CMAKE_CXX_EXTENSIONS OFF )";
}

void FileWriter::writeBuildTargets() {
  headerComment("BUILD TARGETS");

  // Build Targets dropdown
  cmakeLists << "set_property( CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS ";
  for (BuildTarget& target : data->buildTargets()) {
    cmakeLists << inQuotes(target.name()) << ' '; 
  }
  cmakeLists << ')';

  // Default build target
  cmakeLists << "if( NOT " CMAKE_BUILD_CONFIG " )"
    << "\n\tset( CMAKE_BUILD_TYPE " << inQuotes(data->generalData().defaultBuildTarget->name()) << " CACHE STRING \"Project Configuration\" FORCE )"
    << "\nendif()";
  newlines();

  size_t i = 0;
  for (BuildTarget& target : data->buildTargets()) {
    newlines();

    cmakeLists << (i == 0 ? "if" : "elseif")
      << "( " << inQuotes(CMAKE_BUILD_CONFIG) << " STREQUAL " << inQuotes(target.name()) << " )";

    cmakeLists << "\n\tset( CMAKE_CXX_FLAGS \"";
    for (const std::string& flag : target.compilerFlags()) {
      cmakeLists << flag << ' ';
    }
    cmakeLists << "\" )";

    cmakeLists << "\n\tset( CMAKE_C_FLAGS \"";
    for (const std::string& flag : target.compilerFlags()) {
      cmakeLists << flag << ' ';
    }
    cmakeLists << "\" )";

    if (target.hasCompileDefinitions()) {
      cmakeLists << "\n\tadd_compile_definitions( ";

      for (const std::string& definition : target.compilerDefines()) {
        cmakeLists << definition << ' ';
      }
      cmakeLists << ')';
    }
    ++i;
  }

  cmakeLists << "\nendif()"
    << "\n\nmessage( \"Using compiler flags: ${CMAKE_CXX_FLAGS}\" )"
    << "\nmessage( \"Building project '" CMAKE_BUILD_CONFIG << "' configuration\" )";
}

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
