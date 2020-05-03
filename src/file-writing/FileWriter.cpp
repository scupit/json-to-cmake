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

std::string FileWriter::libCreationFunctionString(const OutputItem& lib) {
  return lib.canToggleLibraryType()
    ? "createLibraryWithTypeToggle"
    : "add_library";
}

void FileWriter::labelOutput(OutputItem& output) {
  const std::string toggleableString = output.canToggleLibraryType()
    ? " (toggleable)"
    : "";

  if (output.isExeType()) {
    itemLabel("Output executable: " + output.name());
  }
  else if (output.isLibraryType()) {
    const std::string libType = output.isSharedLibType()
      ? "shared"
      : "static";
    itemLabel("Output " + libType + " library" + toggleableString + ": " + output.name());
  }
}

void FileWriter::writeGeneralOutputData(OutputItem& output) {
  labelOutput(output);

  if (output.hasOrInheritsHeaders()) {
    // Write headers
    cmakeLists << "set( " << headersVariable(output.name());

    if (output.isContainedInGroup()) {
      cmakeLists << "\n\t" << inBraces(headersVariable(output.containingGroup()->getPrefixedName()));
    }

    for (const ImportedLib* linkedLib : output.getAllLinkedImportedLibs()) {
      if (linkedLib->hasHeaders()) {
        cmakeLists << "\n\t" << inBraces(headersVariable(linkedLib->name()));
      }
    }

    for (const OutputItem* linkedOutput : output.getAllLinkedOutputs()) {
      if (linkedOutput->hasOrInheritsHeaders()) {
        cmakeLists << "\n\t" << inBraces(headersVariable(linkedOutput->name()));
      }
    }

    for (const std::string& headerFile : output.headers()) {
      cmakeLists << "\n\t" PROJECT_SOURCE_DIR << '/' << headerFile;
    }

    cmakeLists << "\n)";
    newlines(2);
  }

  // Write sources, which include the item's headers (as cmake variable)
  cmakeLists << "set( " << sourcesVariable(output.name());

  if (output.hasOrInheritsHeaders()) {
    cmakeLists << "\n\t" << inBraces(headersVariable(output.name()));
  }

  if (output.isContainedInGroup() && output.containingGroup()->hasOrInheritsHeaders()) {
    cmakeLists << "\n\t" << inBraces(sourcesVariable(output.containingGroup()->getPrefixedName()));
  }

  if (output.hasMainFile()) {
    cmakeLists << "\n\t" PROJECT_SOURCE_DIR << '/' << output.mainFile();
  }

  for (const std::string& sourceFile : output.sources()) {
    cmakeLists << "\n\t" PROJECT_SOURCE_DIR << '/' << sourceFile;
  }

  newlines(2);

  if (output.hasOrInheritsIncludeDirs()) {
    // Write include dirs
    cmakeLists << "set( " << includeDirsVariable(output.name());

    if (output.isContainedInGroup() && output.containingGroup()->hasOrInheritsIncludeDirs()) {
      cmakeLists << "\n\t" << inBraces(includeDirsVariable(output.containingGroup()->getPrefixedName()));
    }

    for (const ImportedLib* linkedImport : output.getAllLinkedImportedLibs()) {
      if (linkedImport->hasIncludeDirs()) {
        cmakeLists << "\n\t" << inBraces(includeDirsVariable(linkedImport->name()));
      }
    }

    for (const OutputItem* linkedOutput : output.getAllLinkedOutputs()) {
      cmakeLists << "\n\t" << inBraces(includeDirsVariable(linkedOutput->name()));
    }

    for (const std::string& includeDir : output.includeDirs()) {
      cmakeLists << "\n\t" PROJECT_SOURCE_DIR << '/' << includeDir;
    }
    cmakeLists << "\n)";
  }
}

void FileWriter::writeLibOutput(OutputItem& lib, const std::string& type) {
  writeGeneralOutputData(lib); 
  newlines(2);

  cmakeLists << libCreationFunctionString(lib) << "( " << lib.name() << " " << type << " " << inQuotes(inBraces(sourcesVariable(lib.name()))) << " )";
  if (lib.hasOrInheritsIncludeDirs()) {
    cmakeLists << "\ntarget_include_directories( " << lib.name() << " PRIVATE " << inBraces(includeDirsVariable(lib.name())) << " )";
  }
  newlines(2);
  writeOutputDirs(lib);
}

void FileWriter::writeExe(OutputItem& outputExe) {
  writeGeneralOutputData(outputExe);
  newlines(2);

  cmakeLists << "add_executable( " << outputExe.name() << ' ' << inBraces(sourcesVariable(outputExe.name())) << " )";
  if (outputExe.hasOrInheritsIncludeDirs()) {
    cmakeLists << "\ntarget_include_directories( " << outputExe.name() << " PRIVATE " << inBraces(includeDirsVariable(outputExe.name())) << " )";
  }
  newlines(2);
  writeOutputDirs(outputExe);
}

void FileWriter::writeOutputDirs(OutputItem& output) {
  cmakeLists << "set_target_properties( " << output.name() << " PROPERTIES";

  if (output.isLibraryType()) {
    cmakeLists << "\n\tARCHIVE_OUTPUT_DIRECTORY " << getOutputDir(OutputItem::archiveOutputDir)
      << "\n\tLIBRARY_OUTPUT_DIRECTORY " << getOutputDir(OutputItem::libOutputDir);
  }

  if (output.isExeType() || output.isSharedLibType() || output.canToggleLibraryType()) {
    cmakeLists << "\n\tRUNTIME_OUTPUT_DIRECTORY " << getOutputDir(OutputItem::exeOutputDir);
  }
  cmakeLists << "\n)";
}

void FileWriter::writeOutputs() {
  if (data->hasIndividualOutputsOfType(OutputType::SHARED_LIB)) {
    headerComment("INDIVIDUAL OUTPUT SHARED LIBRARIES");

    for (OutputItem& output : data->outputs()) {
      if (output.isSharedLibType()) {
        writeLibOutput(output, "SHARED");
      }
    }
  }

  if (data->hasIndividualOutputsOfType(OutputType::STATIC_LIB)) {
    headerComment("INDIVIDUAL OUTPUT STATIC LIBRARIES");

    for (OutputItem& output : data->outputs()) {
      if (output.isStaticLibType()) {
        writeLibOutput(output, "STATIC");
      }
    }
  }

  writeOutputGroups();

  if (data->hasIndividualOutputsOfType(OutputType::EXE)) {
    headerComment("INDIVIDUAL OUTPUT EXECUTABLES");
    for (OutputItem& output : data->outputs()) {
      if (output.isExeType()) {
        writeExe(output);
      }
    }
  }
}

void FileWriter::writeGeneralGroupData(OutputGroup& group) {
  const std::string prefixedName = group.getPrefixedName();
  itemLabel("Output Group " + prefixedName);

  if (group.hasOrInheritsHeaders()) {
    // Write headers
    cmakeLists << "set( " << headersVariable(prefixedName);
    for (const ImportedLib* linkedImport : group.getAllLinkedImportedLibs()) {
      if (linkedImport->hasHeaders()) {
        cmakeLists << "\n\t" << inBraces(headersVariable(linkedImport->name()));
      }
    }

    for (const OutputItem* linkedOutput : group.getAllLinkedOutputs()) {
      if (linkedOutput->hasOrInheritsHeaders()) {
        cmakeLists << "\n\t" << inBraces(headersVariable(linkedOutput->name()));
      }
    }

    for (const std::string& headerFile : group.headers()) {
      cmakeLists << "\n\t" PROJECT_SOURCE_DIR << '/' << headerFile;
    }
  }

  newlines(2);

  // Write sources, which include the item's headers if they exist
  cmakeLists << "set( " << sourcesVariable(prefixedName);
  if (group.hasOrInheritsHeaders()) {
    cmakeLists << "\n\t" << inBraces(headersVariable(prefixedName));
  }

  for (const std::string& sourceFile : group.sources()) {
    cmakeLists << "\n\t" PROJECT_SOURCE_DIR << '/' << sourceFile;
  }
  cmakeLists << "\n)";

  // Write include dirs
  if (group.hasOrInheritsHeaders()) {
    cmakeLists << "set( " << includeDirsVariable(prefixedName);

    for (const ImportedLib* linkedImport : group.getAllLinkedImportedLibs()) {
      if (linkedImport->hasIncludeDirs()) {
        cmakeLists << "\n\t" << inBraces(includeDirsVariable(linkedImport->name()));
      }
    }

    for (const OutputItem* linkedOutput : group.getAllLinkedOutputs()) {
      if (linkedOutput->hasOrInheritsIncludeDirs()) {
        cmakeLists << "\n\t" << inBraces(includeDirsVariable(linkedOutput->name()));
      }
    }

    for (const std::string& includeDir : group.includeDirs()) {
      cmakeLists << "\n\t" PROJECT_SOURCE_DIR << '/' << includeDir;
    }
    cmakeLists << "\n)";
  }
}

void FileWriter::writeLibraryGroup(OutputGroup& libraryGroup) {
  writeGeneralGroupData(libraryGroup);
  newlines(2);

  for (OutputItem& output : libraryGroup.outputs()) {
    writeLibOutput(output, libraryGroup.isStaticLibType() ? "STATIC" : "SHARED");
  }
}

void FileWriter::writeExeGroup(OutputGroup& exeGroup) {
  writeGeneralGroupData(exeGroup);
  newlines(2); 

  for (OutputItem& output : exeGroup.outputs()) {
    writeExe(output);
  }
}

void FileWriter::writeOutputGroups() {
  if (data->hasLibOutputGroups()) {
    headerComment("LIBRARY OUTPUT GROUPS");
    for (OutputGroup& group : data->outputGroups()) {
      if (group.isLibraryType()) {
        writeLibraryGroup(group);
      }
    }
  } 

  if (data->hasExeOutputGroups()) {
    headerComment("EXECUTABLE OUTPUT GROUPS");
    for (OutputGroup& group : data->outputGroups()) {
      if (group.isExeType()) {
        writeExeGroup(group);
      }
    }
  }
}

void FileWriter::writeLinksForItem(const std::string& targetItemName, const std::vector<OutputItem*> linkedOutputs, const std::vector<ImportedLib*>& linkedImports) {
  cmakeLists << "target_link_libraries( " << targetItemName;

  for (const ImportedLib* linkedImport : linkedImports) {
    for (const std::string& libFileName : linkedImport->libFiles()) {
      cmakeLists << "\n\t" << inBraces(mangleLibName(linkedImport->name(), libFileName));
    }
  }

  for (const OutputItem* linkedOutput : linkedOutputs) {
    cmakeLists << "\n\t" << linkedOutput->name();
  }
  cmakeLists << "\n)";
}

void FileWriter::writeLinks() {
  headerComment("LINK LIBRARIES TO OUTPUTS");

  for (OutputGroup& group : data->outputGroups()) {
    for (OutputItem& output : group.outputs()) {
      if (group.hasLinkedLibs() || output.hasLinkedLibs()) {
        std::vector<OutputItem*> linkedOutputs = output.getAllLinkedOutputs();
        std::vector<ImportedLib*> linkedImports = output.getAllLinkedImportedLibs();

        for (OutputItem* linkedOutput : group.getAllLinkedOutputs()) {
          linkedOutputs.push_back(linkedOutput);
        }
        for (ImportedLib* linkedImport : group.getAllLinkedImportedLibs()) {
          linkedImports.push_back(linkedImport);
        }

        writeLinksForItem(output.name(), linkedOutputs, output.getAllLinkedImportedLibs());
      }
    }
  }

  for (OutputItem& output : data->outputs()) {
    if (output.hasLinkedLibs()) {
      writeLinksForItem(output.name(), output.getAllLinkedOutputs(), output.getAllLinkedImportedLibs());
    }
  }
}

void FileWriter::writeImportedLibCopyCommands() {
  headerComment("IMPORTED LIB COPY COMMANDS");

  for (ImportedLib& importedLib : data->importedLibs()) {
    std::vector<OutputItem*> exesLinkedTo = data->getExesPartOfLinkTree(importedLib);

    if (exesLinkedTo.empty()) {
      if (!data->outputs().empty()) {
        exesLinkedTo.push_back(&data->outputs()[0]);
      }
      else {
        exesLinkedTo.push_back(&data->outputGroups()[0].outputs()[0]);
      }
    }

    for (OutputItem* exeLinkedTo : exesLinkedTo) {
      itemLabel("Copy libraries imported by " + importedLib.name() + " to executable output dir");
      cmakeLists << "add_custom_command(TARGET " << exeLinkedTo->name() << " POST_BUILD"
        << "\n\tCOMMAND ${CMAKE_COMMAND} -E copy_directory"
        << "\n\t\t" PROJECT_SOURCE_DIR "/" << importedLib.dirContainingLibFiles()
        << "\n\t\t" << getOutputDir(OutputItem::exeOutputDir)
        << "\n)";
    }
  }
}
