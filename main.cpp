#include <iostream>
#include <string>
#include <vector>

#include "FileHelper.hpp"
using namespace std;
using namespace FileHelper;

void printVector(const std::vector<std::string>& vec) {
  for (const auto& item : vec) {
    cout << item << endl;
  }
}

int main() {

  // printVector(splitPath(projectRoot().string()));

  cout << resolveFromRoot("../../Something") << endl;

  // cout << filesystem::current_path() << endl;
  return 0;
}