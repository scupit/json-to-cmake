#include <iostream>
#include "AllData.hpp"
#include "helpers/FileHelper.hpp"

using namespace std;

int main(int argc, const char** argv) {
  if (argc > 1) {
    FileHelper::projectRootString.assign(FileHelper::resolve(FileHelper::projectRoot(), argv[1]));
  }
  AllData data(FileHelper::projectRootString.c_str());

  for (auto& item : data.importedLibs()) {
    cout << item.name() << endl;
  }

  return 0;
}