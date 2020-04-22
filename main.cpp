#include <iostream>
#include <string>
#include <set>

#include "JSON/JsonParser.hpp"
#include "output/OutputItem.hpp"
#include "constants/Tags.hpp"

using namespace std;

int main(void) {

  JsonParser jParser("cmake_data.json");
  OutputItem item("gen-cmake", jParser.getJsonReference()[Tags::OUTPUT]["gen-cmake"]);

  // for (const string& str : item.includeDirs()) {
  //   cout << str << endl;
  // }

  cout << item.isLibraryType() << endl;

  return 0;
}