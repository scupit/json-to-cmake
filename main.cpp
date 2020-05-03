#include <filesystem>
#include <iostream>
#include "AllData.hpp"
#include "constants/Globals.hpp"
#include "file-writing/FileWriter.hpp"
#include "helpers/FileHelper.hpp"

using namespace std;
using namespace FileHelper;

int main(int argc, const char** argv) {
  projectRootString.assign(resolve(filesystem::current_path(), argc > 1 ? argv[1] : ""));

  try {
    AllData data;
    FileWriter(&data, joinPath({ projectRootString, Globals::CMAKE_FILE_NAME })).write();
    cout << "CMakeLists.txt written successfully!" << endl;
  }
  catch (std::runtime_error& e) {
    cout << e.what();
  }

  return 0;
}