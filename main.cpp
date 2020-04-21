#include <iostream>
#include <string>
#include <vector>

#include "helpers/FileHelper.hpp"

using namespace std;
using namespace FileHelper;

int main(int argc, const char** argv) {
  forFileInDirRecursive(argc > 1 ? argv[1] : ".", [](const filesystem::path& file) {
    if (file.has_extension() && file.extension() == ".hpp") {
      cout << file << endl;
    }
  });
  return 0;
}