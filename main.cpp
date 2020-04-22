#include <iostream>
#include "AllData.hpp"

using namespace std;

int main(void) {
  AllData data;

  for (auto& item : data.outputGroups()) {
    for (auto& h : item.includeDirs()) {
      cout << h << endl;
    }
  }

  return 0;
}