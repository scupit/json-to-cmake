#include <filesystem>
#include <iostream>
#include "AllData.hpp"
#include "helpers/FileHelper.hpp"

using namespace std;

int main(int argc, const char** argv) {
  FileHelper::projectRootString.assign(FileHelper::resolve(filesystem::current_path(), argc > 1 ? argv[1] : ""));
  AllData data;

  for (auto& item : data.importedLibs()) {
    cout << item.name() << endl;
  }

  return 0;
}