#include <iostream>
#include <string>
#include <set>

#include "FileRetriever.hpp"
#include "JSON/JsonParser.hpp"
#include "constants/Tags.hpp"

using namespace std;

int main(void) {
  set<string> fileNames;
  JsonParser jParser("cmake_data.json");

  FileRetriever::loadSourceFiles(fileNames, jParser.getJsonReference()[Tags::OUTPUT]["gen-cmake"]);

  for (const string& str : fileNames) {
    cout << str << endl;
  }

  return 0;
}