#include <filesystem>
#include <iostream>
#include "AllData.hpp"
#include "helpers/FileHelper.hpp"

using namespace std;

int main(int argc, const char** argv) {
  FileHelper::projectRootString.assign(FileHelper::resolve(filesystem::current_path(), argc > 1 ? argv[1] : ""));

  try {
    AllData data;
    cout << data.generalData().defaultBuildTarget->name() << endl;
  }
  catch (std::runtime_error& e) {
    cout << e.what();
  }

  return 0;
}