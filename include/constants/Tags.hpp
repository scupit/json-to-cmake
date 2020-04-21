#ifndef TAGS_HPP
#define TAGS_HPP

namespace Tags {
	// Main tags
	const char* const PROJECT_NAME = "projectName";

	const char* const OUTPUT = "output";
	const char* const IMPORTED_LIBRARIES = "importedLibs";
	const char* const BUILD_TARGETS = "buildTargets";

	const char* const LINKS = "links";

	const char* const C_STANDARDS = "supportedCStandards";
	const char* const CPP_STANDARDS = "supportedCppStandards";

	const char* const DEFAULT_C_STANDARD = "defaultCStandard";
	const char* const DEFAULT_CPP_STANDARD = "defaultCppStandard";

	const char* const DEFAULT_BUILD_TARGET = "defaultBuildTarget";

	// General item attributes
	const char* const TYPE = "type";

	const char* const R_SOURCE_DIRS = "rSourceDirs";
	const char* const R_HEADER_DIRS = "rHeaderDirs";
	const char* const R_INCLUDE_DIRS = "rIncludeDirs";

	const char* const SOURCE_DIRS = "sourceDirs";
	const char* const HEADER_DIRS = "headerDirs";
	const char* const INCLUDE_DIRS = "includeDirs";

	const char* const INDIVIDUAL_SOURCES = "sources";
	const char* const INDIVIDUAL_HEADERS = "headers";

	// Output attributes
	const char* const MAIN_FILE = "mainFile";

	const char* const EXE_OUTPUT_DIR = "exeOutputDir";
	const char* const LIB_OUTPUT_DIR = "libOutputDir";
	const char* const LIB_TYPE_TOGGLE_POSSIBLE = "canToggleType";

	// Output Types
	const char* const EXE = "executable";
	const char* const SHARED_LIB = "sharedLib";
	const char* const STATIC_LIB = "staticLib";

	// Output Group Atributes
	const char* const OUTPUT_GROUPS = "outputGroups";
	const char* const GROUP_OUTPUTS = "outputs";

	// Imported library attributes
	const char* const IMPORT_ROOT_DIR = "rootDir";
	const char* const IMPORTED_LIB_FILES = "libFiles";
	const char* const IMPORT_GIT_REPO = "gitRepo";
	const char* const IMPORT_SHOULD_CLONE_REPO = "cloneRepo";
	const char* const IMPORT_DOWNLOAD_LINK = "downloadLink";

	const char* const IMPORT_IS_OUTSIDE_PROJECT = "outsideProjectTree";
	const char* const IMPORT_GENERATED_DIRNAME = "generatedDirName";

	// Build Target attributes
	const char* const COMPILER_DEFINES = "defines";
	const char* const COMPILER_FLAGS = "compilerFlags";
}

#endif